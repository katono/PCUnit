#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_Test *current_test;
static int repeat_counter;
static PCU_jmp_buf fatal_jmp;
static int last_assertion;
static int leave_is_enabled;
static int is_verbose;
static int assert_not_flag;
static struct {
	size_t n;
} assert_nstr;
static struct {
	size_t size;
	size_t n;
	size_t idx;
} assert_mem;

static void print_test_name(void);
static void print_result(const char *str);
static void print_file_line_assert(enum PCU_Type type, const char *str_assert, const char *file, unsigned int line);
static void print_repeat(enum PCU_Type type, int repeat);
static void print_params(enum PCU_Type type, PCU_size_t expected, PCU_size_t actual);
static void print_params_double(enum PCU_Type type, double expected, double actual, double delta);

static void PCU_Test_clear_result(PCU_Test *self)
{
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_Test_reset(PCU_Test *self)
{
	PCU_Test_clear_result(self);
}

void PCU_Test_run(PCU_Test *self)
{
	volatile int repeat;
	current_test = self;

	if (is_verbose) {
		print_test_name();
	}
	repeat = (self->ntimes != 0) ? self->ntimes : 1;
	for (repeat_counter = 0; repeat_counter < repeat; repeat_counter++) {
		int err;
		leave_is_enabled = 0;
		err = PCU_Suite_setup();
		if (err) {
			self->result.num_errors++;
			print_file_line_assert(PCU_TYPE_SETUP, "SETUP FAILED", "", (unsigned int) -1);
			print_params(PCU_TYPE_SETUP, 0, (PCU_size_t) err);
			print_repeat(PCU_TYPE_SETUP, repeat_counter);
			continue;
		}
		leave_is_enabled = 1;
		if (PCU_SETJMP(fatal_jmp) == 0) {
			self->test();
		}
		leave_is_enabled = 0;
		err = PCU_Suite_teardown();
		if (err) {
			self->result.num_errors++;
			print_file_line_assert(PCU_TYPE_SETUP, "TEARDOWN FAILED", "", (unsigned int) -1);
			print_params(PCU_TYPE_SETUP, 0, (PCU_size_t) err);
			print_repeat(PCU_TYPE_SETUP, repeat_counter);
			continue;
		}
	}
	if (is_verbose) {
		if (repeat < 0) {
			print_result("SKIPPED");
		} else if (PCU_Test_is_failed(self)) {
			print_result("FAILED");
		} else {
			print_result("PASSED");
		}
	}
}

int PCU_Test_is_skipped(const PCU_Test *self)
{
	return (self->ntimes < 0);
}

int PCU_Test_is_failed(const PCU_Test *self)
{
	return (self->result.num_asserts_failed > 0 || self->result.num_errors > 0);
}

int PCU_test_has_failed(void)
{
	return PCU_Test_is_failed(current_test);
}

void PCU_leave_test_func(void)
{
	PCU_LONGJMP(fatal_jmp, 1);
}

void PCU_assert_impl(int passed_flag, PCU_size_t expected, PCU_size_t actual, 
		enum PCU_Type type, const char *str_assert, const char *file, unsigned int line)
{
	if (passed_flag) {
		last_assertion = 1;
		return;
	}

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	print_file_line_assert(type, str_assert, file, line);
	print_params(type, expected, actual);
	print_repeat(type, repeat_counter);
}

void PCU_assert_num_impl(PCU_size_t expected, PCU_size_t actual, 
		enum PCU_Type type, int not_flag, const char *str_assert, const char *file, unsigned int line)
{
	assert_not_flag = not_flag;
	PCU_assert_impl(not_flag ? (expected != actual) : (expected == actual), 
			expected, actual, type, str_assert, file, line);
}

void PCU_assert_str_impl(const void *expected, const void *actual, size_t n, 
		enum PCU_Type type, int not_flag, const char *str_assert, const char *file, unsigned int line)
{
	int cmp = 0;
	assert_not_flag = not_flag;
	assert_nstr.n = n;
	if (expected == 0 || actual == 0) {
		cmp = (expected != actual);
	} else if (type == PCU_TYPE_STR) {
		cmp = PCU_STRCMP((const char *) expected, (const char *) actual);
	} else if (type == PCU_TYPE_NSTR) {
		cmp = PCU_STRNCMP((const char *) expected, (const char *) actual, n);
	}
#ifdef PCU_USE_WCHAR
	else if (type == PCU_TYPE_STRW) {
		cmp = PCU_WCSCMP((const wchar_t *) expected, (const wchar_t *) actual);
	} else {
		cmp = PCU_WCSNCMP((const wchar_t *) expected, (const wchar_t *) actual, n);
	}
#else
	if (type == PCU_TYPE_STRW || type == PCU_TYPE_NSTRW) {
		cmp = !not_flag;
	}
#endif
	PCU_assert_impl(not_flag ? (cmp != 0) : (cmp == 0), 
			(PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
}

static size_t mem_compare(const void *m1, const void *m2, size_t size, size_t n)
{
	const unsigned char *p1 = (const unsigned char *) m1;
	const unsigned char *p2 = (const unsigned char *) m2;
	size_t len = size * n;
	size_t s = size;
	size_t idx = 0;
	if (len) {
		do {
			if (*p1 != *p2) {
				return idx;
			}
			p1++;
			p2++;
			len--;
			s--;
			if (s == 0) {
				idx++;
				s = size;
			}
		} while (len);
	}
	return (size_t) -1;
}

void PCU_assert_mem_impl(const void *expected, const void *actual, size_t size, size_t n, 
		enum PCU_Type type, int not_flag, const char *str_assert, const char *file, unsigned int line)
{
	int cmp;
	assert_not_flag = not_flag;
	assert_mem.size = size;
	assert_mem.n = n;
	assert_mem.idx = 0;
	if (expected == 0 || actual == 0) {
		cmp = (expected != actual);
	} else {
		size_t idx;
		idx = mem_compare(expected, actual, size, n);
		if (idx == (size_t) -1) {
			cmp = 0;
		} else {
			cmp = 1;
			assert_mem.idx = idx;
		}
	}
	PCU_assert_impl(not_flag ? (cmp != 0) : (cmp == 0), 
			(PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
}

void PCU_assert_double_impl(double expected, double actual, double delta, 
		enum PCU_Type type, int not_flag, const char *str_assert, const char *file, unsigned int line)
{
#ifndef PCU_NO_FLOATINGPOINT
	double dlt = delta;
	assert_not_flag = not_flag;
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
#else
	(void) not_flag;
#endif

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	print_file_line_assert(type, str_assert, file, line);
	print_params_double(type, expected, actual, delta);
	print_repeat(type, repeat_counter);
}

void PCU_assert_op_double_impl(int passed_flag, double expected, double actual, 
		enum PCU_Type type, const char *str_assert, const char *file, unsigned int line)
{
	if (passed_flag) {
		last_assertion = 1;
		return;
	}

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	print_file_line_assert(type, str_assert, file, line);
	print_params_double(type, expected, actual, 0.0);
	print_repeat(type, repeat_counter);
}

void PCU_msg_impl(const char *msg, 
		enum PCU_Type type, const char *str_assert, const char *file, unsigned int line)
{
	current_test->result.num_msgs++;
	print_file_line_assert(type, str_assert, file, line);
	print_params(type, (PCU_size_t)(size_t) msg, 0);
	print_repeat(type, repeat_counter);
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

int PCU_leave_is_enabled(void)
{
	return leave_is_enabled;
}

void PCU_set_verbose(int verbose_flag)
{
	is_verbose = verbose_flag;
}

static int is_first_print(const PCU_Test *self)
{
	if (self->result.num_asserts_failed == 1 && self->result.num_msgs == 0 && 
			self->result.num_errors == 0) {
		return 1;
	} else if (self->result.num_asserts_failed == 0 && self->result.num_msgs == 1 && 
			self->result.num_errors == 0) {
		return 1;
	} else if (self->result.num_asserts_failed == 0 && self->result.num_msgs == 0 && 
			self->result.num_errors == 1) {
		return 1;
	}
	return 0;
}

static void print_test_name(void)
{
	PCU_puts("\nTest: ");
	PCU_puts(PCU_test_name());
	PCU_puts("\n");
}

static void print_result(const char *str)
{
	PCU_puts(" [");
	PCU_puts(str);
	PCU_puts("]\n");
}

static void print_file_line_assert(enum PCU_Type type, const char *str_assert, const char *file, unsigned int line)
{
	if (!is_verbose && is_first_print(current_test)) {
		print_test_name();
	}

	if (type == PCU_TYPE_SETUP) {
		PCU_puts(" ");
		PCU_puts(PCU_test_name());
		PCU_puts(": ");
		PCU_puts(str_assert);
		PCU_puts("\n");
	} else if (type != PCU_TYPE_ADDMSG) {
		PCU_puts(" ");
		PCU_puts(file);
#ifdef _MSC_VER
		PCU_PRINTF1("(%u) : ", line);
#else
		PCU_PRINTF1(":%u: ", line);
#endif
		PCU_puts(str_assert);
		PCU_puts("\n");
	}
}

static int is_repeated_test(const PCU_Test *self)
{
	return (self->ntimes > 1);
}

static void print_repeat(enum PCU_Type type, int repeat)
{
	if (is_repeated_test(current_test) && type != PCU_TYPE_ADDMSG) {
		PCU_PRINTF1("  repeat   : %d\n", repeat);
	}
}

#define IS_ASCII(c)		(' ' == 0x20 && 0x20 <= (c) && (c) <= 0x7e)

#ifdef PCU_DEFINED_LLONG
#define PCU_LX_LD	" : 0x%0*llx (%lld)"
#define PCU_LD	" (%lld)"
#define PCU_LD_LU	" (signed:%lld unsigned:%llu)"
#elif defined(PCU_DEFINED_WIN32_I64)
#define PCU_LX_LD	" : 0x%0*I64x (%I64d)"
#define PCU_LD	" (%I64d)"
#define PCU_LD_LU	" (signed:%I64d unsigned:%I64u)"
#else
#define PCU_LX_LD	" : 0x%0*lx (%ld)"
#define PCU_LD	" (%ld)"
#define PCU_LD_LU	" (signed:%ld unsigned:%lu)"
#endif

static void print_type_num(const char *str, PCU_size_t value, int is_64bit_width)
{
	const int width = (int) ((is_64bit_width ? sizeof(PCU_size_t) : sizeof(size_t)) * 2);
	PCU_puts("  ");
	PCU_puts(str);
	PCU_PRINTF3(PCU_LX_LD, width, value, value);
	if (IS_ASCII(value)) {
		PCU_PRINTF1(" '%c'\n", (int) value);
	} else {
		PCU_puts("\n");
	}
}

#if (defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)) && defined(PCU_NO_DIV32)
static void print_type_num_no_div32(const char *str, PCU_size_t value)
{
	PCU_puts("  ");
	PCU_puts(str);
	PCU_PRINTF2(" : 0x%0*x\n", sizeof(size_t) * 2, value);
}

static void print_expected_actual_no_div32(const char *expected_str, const char *actual_str, 
		PCU_size_t expected, PCU_size_t actual)
{
	if (expected > 0xFFFF || actual > 0xFFFF) {
		print_type_num_no_div32(expected_str, expected);
		print_type_num_no_div32(actual_str  , actual);
	} else {
		print_type_num(expected_str, expected, 0);
		print_type_num(actual_str  , actual  , 0);
	}
}

#else
static void print_expected_actual(const char *expected_str, const char *actual_str, 
		PCU_size_t expected, PCU_size_t actual)
{
	int is_64bit_width = (expected > (PCU_size_t) 0xFFFFFFFF ||
							actual > (PCU_size_t) 0xFFFFFFFF);
	print_type_num(expected_str, expected, is_64bit_width);
	print_type_num(actual_str  , actual  , is_64bit_width);
}

#endif

static void print_null(const char *str)
{
	PCU_puts("  ");
	PCU_puts(str);
	PCU_puts(" : NULL\n");
}

static void print_p(const void *value)
{
#ifdef PCU_NO_STDARG
	PCU_PRINTF1("%p", (size_t) value);
#else
	PCU_PRINTF1("%p", value);
#endif
}

static void print_type_ptr(const char *str, const void *value)
{
	if (value) {
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : ");
		print_p(value);
		PCU_puts("\n");
	} else {
		print_null(str);
	}
}

static void print_type_str(const char *str, const char *value)
{
	if (value) {
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : \"");
		PCU_puts(value);
		PCU_puts("\" (");
		print_p(value);
		PCU_puts(")\n");
	} else {
		print_null(str);
	}
}

static void print_type_nstr(const char *str, const char *value, size_t len)
{
	if (value) {
		size_t i;
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : \"");
		for (i = 0; i < len; i++) {
			PCU_PRINTF1("%c", value[i]);
		}
		PCU_puts("\" (");
		print_p(value);
		PCU_puts(")\n");
	} else {
		print_null(str);
	}
}

#ifdef PCU_USE_WCHAR
static void print_type_nstrw(const char *str, const wchar_t *value, size_t len)
{
	if (value) {
		size_t i;
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : L\"");
		for (i = 0; i < len; i++) {
			PCU_PRINTF1("%lc", value[i]);
		}
		PCU_puts("\" (");
		print_p(value);
		PCU_puts(")\n");
	} else {
		print_null(str);
	}
}

#endif

#if !defined(PCU_USE_WCHAR) || defined(PCU_NO_FLOATINGPOINT)
static void print_not_supported(void)
{
	PCU_puts("  NOT SUPPORTED\n");
}
#endif

static void print_hex(const void *buf, size_t size)
{
	size_t i;
	const unsigned char *p = (const unsigned char *) buf;
	for (i = 0; i < size; i++, p++) {
		PCU_PRINTF1("%02x ", *p);
	}
}

static void print_type_mem(const char *str, const unsigned char *value, size_t size, size_t idx)
{
	if (value) {
		const void *p = value + size * idx;
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : ");
		print_hex(p, size);
		do {
			PCU_ssize_t s_val;
			PCU_size_t  u_val;
			if (size == sizeof(char)) {
				s_val = (PCU_ssize_t) *(const signed char *)p;
				u_val = (PCU_size_t ) *(const unsigned char *)p;
			} else if (size == sizeof(short)) {
				s_val = (PCU_ssize_t) *(const short *)p;
				u_val = (PCU_size_t ) *(const unsigned short *)p;
			} else if (size == sizeof(int)) {
				s_val = (PCU_ssize_t) *(const int *)p;
				u_val = (PCU_size_t ) *(const unsigned int *)p;
#if !((defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)) && defined(PCU_NO_DIV32))
			} else if (size == sizeof(long)) {
				s_val = (PCU_ssize_t) *(const long *)p;
				u_val = (PCU_size_t ) *(const unsigned long *)p;
			} else if (size == sizeof(PCU_size_t)) {
				s_val = (PCU_ssize_t) *(const PCU_ssize_t *)p;
				u_val = (PCU_size_t ) *(const PCU_size_t *)p;
#endif
			} else {
				break;
			}
			if (s_val < 0) {
				PCU_PRINTF2(PCU_LD_LU, s_val, u_val);
			} else {
				PCU_PRINTF1(PCU_LD, s_val);
			}
		} while (0);
		PCU_puts("\n");
	} else {
		print_null(str);
	}
}

static void print_params(enum PCU_Type type, PCU_size_t expected, PCU_size_t actual)
{
	const char * const expected_str = "expected";
	const char * const actual_str   = "actual  ";
	const char * const value1_str = "value1  ";
	const char * const value2_str = "value2  ";
	const char * s1 = expected_str;
	const char * s2 = actual_str;

	if (assert_not_flag) {
		s1 = value1_str;
		s2 = value2_str;
	}
	switch (type) {
	case PCU_TYPE_BOOL:
		PCU_PRINTF1("  expected : %s\n", assert_not_flag ? "TRUE" : "FALSE");
		PCU_PRINTF1("  actual   : %s", assert_not_flag ? "FALSE" : "TRUE");
#if !((defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)) && defined(PCU_NO_DIV32))
		PCU_PRINTF1(PCU_LD, actual);
#endif
		PCU_puts("\n");
		break;
	case PCU_TYPE_NUM:
#if (defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)) && defined(PCU_NO_DIV32)
		print_expected_actual_no_div32(s1, s2, expected, actual);
#else
		print_expected_actual(s1, s2, expected, actual);
#endif
		break;
	case PCU_TYPE_OP:
	case PCU_TYPE_OP_INT:
#if (defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)) && defined(PCU_NO_DIV32)
		print_expected_actual_no_div32(value1_str, value2_str, expected, actual);
#else
		print_expected_actual(value1_str, value2_str, expected, actual);
#endif
		break;
	case PCU_TYPE_PTR:
		print_type_ptr(s1, (const void *)(size_t) expected);
		print_type_ptr(s2  , (const void *)(size_t) actual);
		break;
	case PCU_TYPE_PTR_NULL:
		PCU_PRINTF1("  expected : %sNULL\n", assert_not_flag ? "non-" : "");
		print_type_ptr(actual_str, (const void *)(size_t) actual);
		break;
	case PCU_TYPE_STR:
		print_type_str(s1, (const char *)(size_t) expected);
		print_type_str(s2  , (const char *)(size_t) actual);
		break;
	case PCU_TYPE_MEM:
		if (assert_mem.n > 1) {
			PCU_PRINTF1("  nth      : %u\n", (unsigned int) assert_mem.idx);
		}
		print_type_mem(s1, (const unsigned char *)(size_t) expected, assert_mem.size, assert_mem.idx);
		print_type_mem(s2, (const unsigned char *)(size_t) actual, assert_mem.size, assert_mem.idx);
		PCU_PRINTF1("  size     : %u\n", (unsigned int) assert_mem.size);
		break;
	case PCU_TYPE_NSTR:
		print_type_nstr(s1, (const char *)(size_t) expected, assert_nstr.n);
		print_type_nstr(s2  , (const char *)(size_t) actual, assert_nstr.n);
		PCU_PRINTF1("  length   : %u\n", (unsigned int) assert_nstr.n);
		break;
#ifdef PCU_USE_WCHAR
	case PCU_TYPE_STRW:
		print_type_nstrw(s1, (const wchar_t *)(size_t) expected, 
				expected ? PCU_WCSLEN((const wchar_t *)(size_t) expected) : 0);
		print_type_nstrw(s2  , (const wchar_t *)(size_t) actual, 
				actual ? PCU_WCSLEN((const wchar_t *)(size_t) actual) : 0);
		break;
	case PCU_TYPE_NSTRW:
		print_type_nstrw(s1, (const wchar_t *)(size_t) expected, assert_nstr.n);
		print_type_nstrw(s2  , (const wchar_t *)(size_t) actual, assert_nstr.n);
		PCU_PRINTF1("  length   : %u\n", (unsigned int) assert_nstr.n);
		break;
#else
	case PCU_TYPE_STRW:
	case PCU_TYPE_NSTRW:
		print_not_supported();
		break;
#endif
	case PCU_TYPE_MSG:
	case PCU_TYPE_ADDMSG:
	case PCU_TYPE_FAIL:
		PCU_puts("  ");
		PCU_puts((const char *)(size_t) expected);
		PCU_puts("\n");
		break;
	case PCU_TYPE_SETUP:
		PCU_PRINTF1("  return   : %d\n", (int) actual);
		break;
	default:
		break;
	}
}

static void print_params_double(enum PCU_Type type, double expected, double actual, double delta)
{
#ifndef PCU_NO_FLOATINGPOINT
	switch (type) {
	case PCU_TYPE_DBL:
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
		PCU_PRINTF1("  expected : |value1 - value2| %s |delta|\n", assert_not_flag ? ">" : "<=");
		PCU_PRINTF1("  value1   : %g\n", expected);
		PCU_PRINTF1("  value2   : %g\n", actual);
		PCU_PRINTF1("  delta    : %g\n", delta);
#else
		(void) expected;
		(void) actual;
		(void) delta;
#endif
		break;
	case PCU_TYPE_OP_DBL:
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
		PCU_PRINTF1("  value1   : %g\n", expected);
		PCU_PRINTF1("  value2   : %g\n", actual);
#endif
		break;
	default:
		break;
	}
#else
	(void) type;
	(void) expected;
	(void) actual;
	(void) delta;
	print_not_supported();
#endif
}

#ifndef PCU_NO_CONSOLE_RUN
int PCU_is_verbose(void)
{
	return is_verbose;
}
#endif

