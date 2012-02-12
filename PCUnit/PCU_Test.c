#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_Test *current_test;
static int repeat_counter;
static PCU_jmp_buf fatal_jmp;
static int last_assertion;

static PCU_TestFailure *PCU_TestFailure_new(PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int repeat);
#ifndef PCU_NO_FLOATINGPOINT
static PCU_TestFailure *PCU_TestFailure_new_double(double expected, double actual, double delta, unsigned long type, const char *str_assert, const char *file, unsigned int line, int repeat);
#endif
static void PCU_TestFailure_delete(PCU_TestFailure *self);
static void list_push(PCU_TestFailureList *list, PCU_TestFailure *node);
static PCU_TestFailure *list_pop(PCU_TestFailureList *list);
#define LIST_EMPTY(list)	((list)->head == 0)

static void PCU_Test_clear_result(PCU_Test *self)
{
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_Test_reset(PCU_Test *self, const PCU_Suite *suite)
{
	self->suite = suite;
	PCU_TestFailureList *list = &self->failure_list;
	while (!LIST_EMPTY(list)) {
		PCU_TestFailure *tmp = list_pop(list);
		PCU_TestFailure_delete(tmp);
	}
	PCU_Test_clear_result(self);
}

static int PCU_Test_setup(const PCU_Test *self)
{
	int ret = 0;
	if (self->suite->setup) {
		ret = self->suite->setup();
	}
	return ret;
}

static int PCU_Test_teardown(const PCU_Test *self)
{
	int ret = 0;
	if (self->suite->teardown) {
		ret = self->suite->teardown();
	}
	return ret;
}

void PCU_Test_run(PCU_Test *self)
{
	volatile int repeat;
	current_test = self;

	repeat = (self->ntimes != 0) ? self->ntimes : 1;
	for (repeat_counter = 0; repeat_counter < repeat; repeat_counter++) {
		int err;
		err = PCU_Test_setup(self);
		if (err) {
			PCU_TestFailure *node;
			self->result.num_errors_setup++;
			node = PCU_TestFailure_new(0, (PCU_size_t) err, PCU_TYPE_SETUP, "SETUP FAILED", "", (unsigned int) -1, repeat_counter);
			if (node) {
				list_push(&self->failure_list, node);
			}
			continue;
		}
		if (PCU_SETJMP(fatal_jmp) == 0) {
			self->test();
		}
		err = PCU_Test_teardown(self);
		if (err) {
			PCU_TestFailure *node;
			self->result.num_errors_teardown++;
			node = PCU_TestFailure_new(0, (PCU_size_t) err, PCU_TYPE_SETUP, "TEARDOWN FAILED", "", (unsigned int) -1, repeat_counter);
			if (node) {
				list_push(&self->failure_list, node);
			}
			continue;
		}
	}
}

void PCU_Test_get_result(PCU_Test *self, PCU_TestResult *result)
{
	PCU_MEMCPY(result, &self->result, sizeof *result);
}

int PCU_Test_is_repeated_test(PCU_Test *self)
{
	return (self->ntimes > 1);
}

int PCU_Test_is_skipped(PCU_Test *self)
{
	return (self->ntimes < 0);
}

int PCU_Test_is_failed(PCU_Test *self)
{
	return (self->result.num_asserts_failed > 0 || self->result.num_errors_setup > 0 || self->result.num_errors_teardown > 0);
}

int PCU_Test_contains_msg(PCU_Test *self)
{
	return (self->result.num_msgs > 0);
}

void PCU_leave_test_func(void)
{
	PCU_LONGJMP(fatal_jmp, 1);
}

void PCU_assert_impl(int passed_flag, PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	PCU_TestFailure *node;
	current_test->result.num_asserts++;
	current_test->result.num_asserts_ran++;

	if (passed_flag) {
		last_assertion = 1;
		return;
	}

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	node = PCU_TestFailure_new(expected, actual, type, str_assert, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
}

void PCU_assert_num_impl(PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	if (PCU_get_not_flag(type)) {
		PCU_assert_impl((expected != actual), expected, actual, type, str_assert, file, line);
	} else {
		PCU_assert_impl((expected == actual), expected, actual, type, str_assert, file, line);
	}
}

void PCU_assert_ptr_impl(const void *expected, const void *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	if (PCU_get_not_flag(type)) {
		PCU_assert_impl((expected != actual), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
	} else {
		PCU_assert_impl((expected == actual), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
	}
}

void PCU_assert_str_impl(const char *expected, const char *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	if (expected == 0 || actual == 0) {
		PCU_assert_impl(0, (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		return;
	}
	if (PCU_get_assert_type(type) == PCU_TYPE_NSTR) {
		size_t len = PCU_get_nstr_len(type);
		if (PCU_get_not_flag(type)) {
			PCU_assert_impl((PCU_STRNCMP(expected, actual, len) != 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		} else {
			PCU_assert_impl((PCU_STRNCMP(expected, actual, len) == 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		}
	} else {
		if (PCU_get_not_flag(type)) {
			PCU_assert_impl((PCU_STRCMP(expected, actual) != 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		} else {
			PCU_assert_impl((PCU_STRCMP(expected, actual) == 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		}
	}
}

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
void PCU_assert_strw_impl(const wchar_t *expected, const wchar_t *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	if (expected == 0 || actual == 0) {
		PCU_assert_impl(0, (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		return;
	}
	if (PCU_get_assert_type(type) == PCU_TYPE_NSTRW) {
		size_t len = PCU_get_nstr_len(type);
		if (PCU_get_not_flag(type)) {
			PCU_assert_impl((PCU_WCSNCMP(expected, actual, len) != 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		} else {
			PCU_assert_impl((PCU_WCSNCMP(expected, actual, len) == 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		}
	} else {
		if (PCU_get_not_flag(type)) {
			PCU_assert_impl((PCU_WCSCMP(expected, actual) != 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		} else {
			PCU_assert_impl((PCU_WCSCMP(expected, actual) == 0), (PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
		}
	}
}
#endif

#ifndef PCU_NO_FLOATINGPOINT
void PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	double dlt = delta;
	int not_flag;
	PCU_TestFailure *node;
	current_test->result.num_asserts++;
	current_test->result.num_asserts_ran++;

	not_flag = PCU_get_not_flag(type);
	if (dlt < 0) {
		dlt = -dlt;
	}

	if (expected == actual) {
		if (!not_flag) {
			last_assertion = 1;
			return;
		}
	} else {
		double d = (expected < actual) ? actual - expected : expected - actual;
		if (!not_flag && d <= dlt) {
			last_assertion = 1;
			return;
		} else if (not_flag && d > dlt) {
			last_assertion = 1;
			return;
		}
	}

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	node = PCU_TestFailure_new_double(expected, actual, delta, type, str_assert, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
}

void PCU_assert_op_double_impl(int passed_flag, double expected, double actual, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	PCU_TestFailure *node;
	current_test->result.num_asserts++;
	current_test->result.num_asserts_ran++;

	if (passed_flag) {
		last_assertion = 1;
		return;
	}

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	node = PCU_TestFailure_new_double(expected, actual, 0.0, type, str_assert, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
}
#endif

void PCU_msg_impl(const char *msg, unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	PCU_TestFailure *node;
	current_test->result.num_msgs++;
	node = PCU_TestFailure_new((PCU_size_t)(size_t) msg, 0, type, str_assert, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
}

int PCU_repeat_counter(void)
{
	return repeat_counter;
}

const char *PCU_test_name(void)
{
	return current_test->name;
}

int PCU_last_assertion(void)
{
	return last_assertion;
}

static int copy_string(char **dst1, char **dst2, const char *src1, const char *src2, unsigned long type)
{
	char *p1;
	char *p2;
	size_t dst1_len;
	size_t dst2_len;
	size_t len = 0;
	unsigned long t;
	if (!src1 && !src2) {
		*dst1 = 0;
		*dst2 = 0;
		return 1;
	}
	t = PCU_get_assert_type(type);
	if (t == PCU_TYPE_NSTR) {
		len = PCU_get_nstr_len(type);
		dst1_len = src1 ? len + 1 : 0;
		dst2_len = src2 ? len + 1 : 0;
	} else {
		dst1_len = src1 ? PCU_STRLEN(src1) + 1 : 0;
		dst2_len = src2 ? PCU_STRLEN(src2) + 1 : 0;
	}
	p1 = (char *) PCU_STR_MALLOC(sizeof(char) * (dst1_len + dst2_len));
	if (!p1) {
		return 0;
	}
	p2 = p1 + dst1_len;
	if (t == PCU_TYPE_NSTR) {
		if (src1) {
			PCU_STRNCPY(p1, src1, len);
			p1[len] = '\0';
		}
		if (src2) {
			PCU_STRNCPY(p2, src2, len);
			p2[len] = '\0';
		}
	} else {
		if (src1) {
			PCU_STRCPY(p1, src1);
		}
		if (src2) {
			PCU_STRCPY(p2, src2);
		}
	}
	*dst1 = src1 ? p1 : 0;
	*dst2 = src2 ? p2 : 0;
	return 1;
}

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
static size_t get_mblen_nstrw(const wchar_t *wstr, size_t wstr_len)
{
	size_t ret = 0;
	if (wstr) {
		size_t i;
		for (i = 0; i < wstr_len; i++) {
			size_t s;
			wchar_t tmp[2];
			tmp[0] = wstr[i];
			tmp[1] = 0;
			s = PCU_WCSTOMBS(0, tmp, 0);
			if (s == (size_t) -1) {
				ret = MB_CUR_MAX * PCU_WCSLEN(wstr);
				break;
			} else if (s == 1 && tmp[0] == 0) {
				break;
			}
			ret += s;
		}
		ret++;
	}
	return ret;
}

static size_t get_mblen_strw(const wchar_t *wstr)
{
	size_t ret = 0;
	if (wstr) {
		ret = PCU_WCSTOMBS(0, wstr, 0);
		if (ret == (size_t) -1) {
			ret = MB_CUR_MAX * PCU_WCSLEN(wstr);
		}
		ret++;
	}
	return ret;
}

static void to_mbs(char *cstr, const wchar_t *wstr, size_t cstr_len)
{
	if (wstr) {
		PCU_WCSTOMBS(cstr, wstr, cstr_len);
		cstr[cstr_len - 1] = '\0';
	}
}

static int copy_stringw(char **dst1, char **dst2, const wchar_t *src1, const wchar_t *src2, unsigned long type)
{
	char *p1;
	char *p2;
	size_t dst1_len;
	size_t dst2_len;
	size_t len = 0;
	unsigned long t;
	if (!src1 && !src2) {
		*dst1 = 0;
		*dst2 = 0;
		return 1;
	}
	t = PCU_get_assert_type(type);
	if (t == PCU_TYPE_NSTRW) {
		len = PCU_get_nstr_len(type);
		dst1_len = get_mblen_nstrw(src1, len);
		dst2_len = get_mblen_nstrw(src2, len);
	} else {
		dst1_len = get_mblen_strw(src1);
		dst2_len = get_mblen_strw(src2);
	}
	p1 = (char *) PCU_STR_MALLOC(sizeof(char) * (dst1_len + dst2_len));
	if (!p1) {
		return 0;
	}
	p2 = p1 + dst1_len;
	to_mbs(p1, src1, dst1_len);
	to_mbs(p2, src2, dst2_len);
	*dst1 = src1 ? p1 : 0;
	*dst2 = src2 ? p2 : 0;
	return 1;
}

const char *PCU_formatW(const wchar_t *format, ...)
{
	extern char PCU_msg_buf[PCU_MESSAGE_BUF_SIZE];
	static wchar_t PCU_msgw_buf[PCU_MESSAGE_BUF_SIZE];
	va_list ap;
	va_start(ap, format);
	PCU_VSWPRINTF(PCU_msgw_buf, PCU_MESSAGE_BUF_SIZE, format, ap);
	va_end(ap);
	to_mbs(PCU_msg_buf, PCU_msgw_buf, PCU_MESSAGE_BUF_SIZE);
	return PCU_msg_buf;
}

#endif

static void print_malloc_failed(const char *str_assert, const char *file, unsigned int line)
{
	PCU_PRINTF0("malloc failed: ");
	PCU_PRINTF1("%s", file);
	PCU_PRINTF1(":%u: ", line);
	PCU_PRINTF1("%s", str_assert);
	PCU_PRINTF0("\n");
}

static PCU_TestFailure *PCU_TestFailure_new(PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int repeat)
{
	PCU_TestFailure *self = (PCU_TestFailure *) PCU_MALLOC(sizeof(PCU_TestFailure));
	if (!self) {
		print_malloc_failed(str_assert, file, line);
		return 0;
	}

	self->type = type;
	switch (PCU_get_assert_type(type)) {
	case PCU_TYPE_NONE:
		break;
	case PCU_TYPE_BOOL:
	case PCU_TYPE_NUM:
	case PCU_TYPE_OP:
	case PCU_TYPE_OP_INT:
	case PCU_TYPE_SETUP:
		self->expected.num = expected;
		self->actual.num = actual;
		break;
	case PCU_TYPE_PTR:
	case PCU_TYPE_PTR_NULL:
		self->expected.ptr = (const void *)(size_t) expected;
		self->actual.ptr = (const void *)(size_t) actual;
		break;
	case PCU_TYPE_STR:
	case PCU_TYPE_NSTR:
	case PCU_TYPE_MSG:
	case PCU_TYPE_ADDMSG:
	case PCU_TYPE_FAIL:
		if (!copy_string(&self->expected.str, &self->actual.str, (const char *)(size_t) expected, (const char *)(size_t) actual, type)) {
			self->actual.num = (PCU_size_t) -1; /* actual.num is used as str_malloc_failed_flag */
		}
		break;
#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
	case PCU_TYPE_STRW:
	case PCU_TYPE_NSTRW:
		if (!copy_stringw(&self->expected.str, &self->actual.str, (const wchar_t *)(size_t) expected, (const wchar_t *)(size_t) actual, type)) {
			self->actual.num = (PCU_size_t) -1; /* actual.num is used as str_malloc_failed_flag */
		}
		break;
#endif
	default:
		self->type = PCU_TYPE_NONE;
		break;
	}

	self->str_assert = str_assert;
	self->file = file;
	self->line = line;
	self->repeat = repeat;
	self->next = 0;
	return self;
}

#ifndef PCU_NO_FLOATINGPOINT
static PCU_TestFailure *PCU_TestFailure_new_double(double expected, double actual, double delta, unsigned long type, const char *str_assert, const char *file, unsigned int line, int repeat)
{
	PCU_TestFailure *self = (PCU_TestFailure *) PCU_MALLOC(sizeof(PCU_TestFailure));
	if (!self) {
		print_malloc_failed(str_assert, file, line);
		return 0;
	}

	self->type = type;
	switch (PCU_get_assert_type(type)) {
	case PCU_TYPE_DBL:
	case PCU_TYPE_OP_DBL:
#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		self->expected.dbl = expected;
		self->actual.dbl = actual;
		self->delta = delta;
#else
		(void) expected;
		(void) actual;
		(void) delta;
#endif
		break;
	default:
		self->type = PCU_TYPE_NONE;
		break;
	}

	self->str_assert = str_assert;
	self->file = file;
	self->line = line;
	self->repeat = repeat;
	self->next = 0;
	return self;
}
#endif

static void PCU_TestFailure_delete(PCU_TestFailure *self)
{
	if (!self) return;
	switch (PCU_get_assert_type(self->type)) {
	case PCU_TYPE_STR:
	case PCU_TYPE_NSTR:
	case PCU_TYPE_MSG:
	case PCU_TYPE_ADDMSG:
	case PCU_TYPE_FAIL:
#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
	case PCU_TYPE_STRW:
	case PCU_TYPE_NSTRW:
#endif
		if (!PCU_TestFailure_str_malloc_is_failed(self)) {
			if (self->expected.str) {
				PCU_STR_FREE(self->expected.str);
			} else {
				PCU_STR_FREE(self->actual.str);
			}
		}
		break;
	default:
		break;
	}
	PCU_FREE(self);
}

int PCU_TestFailure_str_malloc_is_failed(PCU_TestFailure *self)
{
	return self->actual.num == (PCU_size_t) -1;
}

static void list_push(PCU_TestFailureList *list, PCU_TestFailure *node)
{
	node->next = 0;
	if (LIST_EMPTY(list)) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
}

static PCU_TestFailure *list_pop(PCU_TestFailureList *list)
{
	PCU_TestFailure *node = list->head;
	list->head = list->head->next;
	if (list->head == 0) {
		list->tail = 0;
	}
	return node;
}

