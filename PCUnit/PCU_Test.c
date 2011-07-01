#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_Test *current_test;
static int repeat_counter;
static PCU_jmp_buf fatal_jmp;

static PCU_TestFailure *PCU_TestFailure_new(size_t expected, size_t actual, unsigned long type, const char *expr, const char *file, unsigned int line, int repeat);
#ifndef PCU_NO_FLOATINGPOINT
static PCU_TestFailure *PCU_TestFailure_new_double(double expected, double actual, double delta, unsigned long type, const char *expr, const char *file, unsigned int line, int repeat);
#endif
static void PCU_TestFailure_delete(PCU_TestFailure *self);
static void list_push(PCU_TestFailure *list, PCU_TestFailure *node);
static PCU_TestFailure *list_pop(PCU_TestFailure *list);
#define LIST_EMPTY(list)	((list)->next == list)

static void PCU_Test_clear_result(PCU_Test *self)
{
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_Test_reset(PCU_Test *self, const PCU_Suite *suite)
{
	self->suite = suite;
	if (!self->failure_list.next) {
		self->failure_list.next = &self->failure_list;
		self->failure_list.prev = &self->failure_list;
	} else {
		PCU_TestFailure *list = &self->failure_list;
		while (!LIST_EMPTY(list)) {
			PCU_TestFailure *tmp = list_pop(list);
			PCU_TestFailure_delete(tmp);
		}
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
	int repeat;
	current_test = self;

	repeat = (self->repeat != 0) ? self->repeat : 1;
	for (repeat_counter = 0; repeat_counter < repeat; repeat_counter++) {
		int err;
		err = PCU_Test_setup(self);
		if (err) {
			PCU_TestFailure *node;
			self->result.num_errors_setup++;
			node = PCU_TestFailure_new(0, (size_t) err, PCU_TYPE_SETUP, "SETUP FAILED", "", (unsigned int) -1, repeat_counter);
			if (node) {
				list_push(&current_test->failure_list, node);
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
			node = PCU_TestFailure_new(0, (size_t) err, PCU_TYPE_SETUP, "TEARDOWN FAILED", "", (unsigned int) -1, repeat_counter);
			if (node) {
				list_push(&current_test->failure_list, node);
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
	if (self->repeat > 0) {
		return 1;
	}
	return 0;
}

int PCU_Test_is_failed(PCU_Test *self)
{
	if (self->result.num_asserts_failed > 0 || self->result.num_errors_setup > 0 || self->result.num_errors_teardown > 0) {
		return 1;
	}
	return 0;
}

int PCU_assert_impl(int passed_flag, size_t expected, size_t actual, unsigned long type, const char *expr, const char *file, unsigned int line, int fatal_flag)
{
	PCU_TestFailure *node;
	current_test->result.num_asserts++;
	current_test->result.num_asserts_ran++;

	if (passed_flag) {
		return 1;
	}

	current_test->result.num_asserts_failed++;

	node = PCU_TestFailure_new(expected, actual, type, expr, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
	if (fatal_flag) {
		PCU_LONGJMP(fatal_jmp, 1);
	}
	return 0;
}

#ifndef PCU_NO_FLOATINGPOINT
int PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *expr, const char *file, unsigned int line, int fatal_flag)
{
	double dlt = delta;
	int not_flag;
	PCU_TestFailure *node;
	current_test->result.num_asserts++;
	current_test->result.num_asserts_ran++;

	not_flag = PCU_GET_NOT_FLAG(type);
	if (dlt < 0) {
		dlt = -dlt;
	}

	if (expected == actual) {
		if (!not_flag) {
			return 1;
		}
	} else {
		double d = (expected < actual) ? actual - expected : expected - actual;
		if (!not_flag && d <= dlt) {
			return 1;
		} else if (not_flag && d > dlt) {
			return 1;
		}
	}

	current_test->result.num_asserts_failed++;

	node = PCU_TestFailure_new_double(expected, actual, delta, type, expr, file, line, repeat_counter);
	if (node) {
		list_push(&current_test->failure_list, node);
	}
	if (fatal_flag) {
		PCU_LONGJMP(fatal_jmp, 1);
	}
	return 0;
}
#endif

void PCU_msg_impl(const char *msg, const char *file, unsigned int line)
{
	PCU_TestFailure *node;
	node = PCU_TestFailure_new((size_t) msg, 0, PCU_TYPE_MSG, "PCU_MSG", file, line, repeat_counter);
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

static int copy_string(char **dst1, char **dst2, const char *src1, const char *src2, unsigned long type)
{
	char *p1;
	char *p2;
	size_t src1_len;
	size_t src2_len;
	size_t len = 0;
	unsigned long t;
	if (!src1 && !src2) {
		*dst1 = 0;
		*dst2 = 0;
		return 1;
	}
	t = PCU_GET_ASSERT_TYPE(type);
	if (t == PCU_TYPE_NSTR) {
		len = PCU_GET_NSTR_LEN(type);
		src1_len = src1 ? len + 1 : 0;
		src2_len = src2 ? len + 1 : 0;
	} else {
		src1_len = src1 ? PCU_STRLEN(src1) + 1 : 0;
		src2_len = src2 ? PCU_STRLEN(src2) + 1 : 0;
	}
	p1 = (char *) PCU_STR_MALLOC(sizeof(char) * (src1_len + src2_len));
	if (!p1) {
		return 0;
	}
	p2 = p1 + src1_len;
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

static PCU_TestFailure *PCU_TestFailure_new(size_t expected, size_t actual, unsigned long type, const char *expr, const char *file, unsigned int line, int repeat)
{
	PCU_TestFailure *self = (PCU_TestFailure *) PCU_MALLOC(sizeof(PCU_TestFailure));
	if (!self) {
		PCU_PRINTF3("malloc failed: %s(%u): %s\n", file, line, expr);
		return 0;
	}

	self->type = type;
	switch (PCU_GET_ASSERT_TYPE(type)) {
	case PCU_TYPE_NONE:
		break;
	case PCU_TYPE_BOOL:
	case PCU_TYPE_NUM_CHAR:
	case PCU_TYPE_NUM_SHORT:
	case PCU_TYPE_NUM_INT:
	case PCU_TYPE_NUM_LONG:
	case PCU_TYPE_NUM_SIZET:
	case PCU_TYPE_NUM_LLONG:
	case PCU_TYPE_OP_CHAR:
	case PCU_TYPE_OP_SHORT:
	case PCU_TYPE_OP_INT:
	case PCU_TYPE_OP_LONG:
	case PCU_TYPE_OP_SIZET:
	case PCU_TYPE_OP_LLONG:
	case PCU_TYPE_SETUP:
		self->expected.num = expected;
		self->actual.num = actual;
		break;
	case PCU_TYPE_PTR:
		self->expected.ptr = (const void *) expected;
		self->actual.ptr = (const void *) actual;
		break;
	case PCU_TYPE_STR:
	case PCU_TYPE_NSTR:
	case PCU_TYPE_MSG:
	case PCU_TYPE_FAIL:
		if (!copy_string(&self->expected.str, &self->actual.str, (const char *) expected, (const char *) actual, type)) {
			PCU_PRINTF3("string malloc failed: %s(%u): %s\n", file, line, expr);
			PCU_FREE(self);
			return 0;
		}
		break;
	default:
		self->type = PCU_TYPE_NONE;
		break;
	}

	self->expr = expr;
	self->file = file;
	self->line = line;
	self->repeat = repeat;
	self->next = self->prev = 0;
	return self;
}

#ifndef PCU_NO_FLOATINGPOINT
static PCU_TestFailure *PCU_TestFailure_new_double(double expected, double actual, double delta, unsigned long type, const char *expr, const char *file, unsigned int line, int repeat)
{
	PCU_TestFailure *self = (PCU_TestFailure *) PCU_MALLOC(sizeof(PCU_TestFailure));
	if (!self) {
		PCU_PRINTF3("malloc failed: %s(%u): %s\n", file, line, expr);
		return 0;
	}

	self->type = type;
	switch (PCU_GET_ASSERT_TYPE(type)) {
	case PCU_TYPE_DBL:
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

	self->expr = expr;
	self->file = file;
	self->line = line;
	self->repeat = repeat;
	self->next = self->prev = 0;
	return self;
}
#endif

static void PCU_TestFailure_delete(PCU_TestFailure *self)
{
	if (!self) return;
	switch (PCU_GET_ASSERT_TYPE(self->type)) {
	case PCU_TYPE_STR:
	case PCU_TYPE_NSTR:
	case PCU_TYPE_MSG:
	case PCU_TYPE_FAIL:
		if (self->expected.str) {
			PCU_STR_FREE(self->expected.str);
		} else {
			PCU_STR_FREE(self->actual.str);
		}
		break;
	default:
		break;
	}
	PCU_FREE(self);
}

static void list_push(PCU_TestFailure *list, PCU_TestFailure *node)
{
	node->prev = list->prev;
	node->next = list;
	list->prev->next = node;
	list->prev = node;
}

static PCU_TestFailure *list_pop(PCU_TestFailure *list)
{
	PCU_TestFailure *node;
	node = list->next;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	return node;
}

