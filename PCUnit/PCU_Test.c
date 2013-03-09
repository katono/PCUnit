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
static int is_verbose;

static void print_test_name(void);
static void print_result(const char *str);
static void print_file_line_assert(unsigned long type, const char *str_assert, const char *file, unsigned int line);
static void print_repeat(unsigned long type, int repeat);
static void print_params(unsigned long type, PCU_size_t expected, PCU_size_t actual);
static void print_params_double(unsigned long type, double expected, double actual, double delta);

#define PCU_TYPE_NOT_NSTR	(~(PCU_TYPE_NSTR | PCU_TYPE_NSTRW | PCU_TYPE_NOT))

static unsigned long get_assert_type(unsigned long type)
{
	if (type & PCU_TYPE_NSTR) {
		return PCU_TYPE_NSTR;
	} else if (type & PCU_TYPE_NSTRW) {
		return PCU_TYPE_NSTRW;
	} else {
		return (type & PCU_TYPE_NOT_NSTR);
	}
}

static size_t get_nstr_len(unsigned long type)
{
	return (size_t) (type & PCU_TYPE_NOT_NSTR);
}

static int get_not_flag(unsigned long type)
{
	return (type & PCU_TYPE_NOT) ? 1 : 0;
}

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
		err = PCU_Suite_setup();
		if (err) {
			self->result.num_errors++;
			print_file_line_assert(PCU_TYPE_SETUP, "SETUP FAILED", "", (unsigned int) -1);
			print_params(PCU_TYPE_SETUP, 0, (PCU_size_t) err);
			print_repeat(PCU_TYPE_SETUP, repeat_counter);
			continue;
		}
		if (PCU_SETJMP(fatal_jmp) == 0) {
			self->test();
		}
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

void PCU_leave_test_func(void)
{
	PCU_LONGJMP(fatal_jmp, 1);
}

void PCU_assert_impl(int passed_flag, PCU_size_t expected, PCU_size_t actual, 
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
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
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	PCU_assert_impl(get_not_flag(type) ? (expected != actual) : (expected == actual), 
			expected, actual, type, str_assert, file, line);
}

void PCU_assert_str_impl(const char *expected, const char *actual, 
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	int cmp;
	if (expected == 0 || actual == 0) {
		cmp = (expected != actual);
	} else if (get_assert_type(type) == PCU_TYPE_NSTR) {
		cmp = PCU_STRNCMP(expected, actual, get_nstr_len(type));
	} else {
		cmp = PCU_STRCMP(expected, actual);
	}
	PCU_assert_impl(get_not_flag(type) ? (cmp != 0) : (cmp == 0), 
			(PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
}

void PCU_assert_strw_impl(const void *expected, const void *actual, 
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
#ifdef PCU_USE_WCHAR
	int cmp;
	if (expected == 0 || actual == 0) {
		cmp = (expected != actual);
	} else if (get_assert_type(type) == PCU_TYPE_NSTRW) {
		cmp = PCU_WCSNCMP((const wchar_t *) expected, (const wchar_t *) actual, get_nstr_len(type));
	} else {
		cmp = PCU_WCSCMP((const wchar_t *) expected, (const wchar_t *) actual);
	}
	PCU_assert_impl(get_not_flag(type) ? (cmp != 0) : (cmp == 0), 
			(PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
#else
	PCU_assert_impl(0, 
			(PCU_size_t)(size_t) expected, (PCU_size_t)(size_t) actual, type, str_assert, file, line);
#endif
}

void PCU_assert_double_impl(double expected, double actual, double delta, 
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
#ifndef PCU_NO_FLOATINGPOINT
	double dlt = delta;
	int not_flag;
	not_flag = get_not_flag(type);
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
#endif

	last_assertion = 0;
	current_test->result.num_asserts_failed++;

	print_file_line_assert(type, str_assert, file, line);
	print_params_double(type, expected, actual, delta);
	print_repeat(type, repeat_counter);
}

void PCU_assert_op_double_impl(int passed_flag, double expected, double actual, 
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
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
		unsigned long type, const char *str_assert, const char *file, unsigned int line)
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

static void print_file_line_assert(unsigned long type, const char *str_assert, const char *file, unsigned int line)
{
	unsigned long t;
	if (!is_verbose && is_first_print(current_test)) {
		print_test_name();
	}

	t = get_assert_type(type);
	if (t == PCU_TYPE_SETUP) {
		PCU_puts(" ");
		PCU_puts(PCU_test_name());
		PCU_puts(": ");
		PCU_puts(str_assert);
		PCU_puts("\n");
	} else if (t != PCU_TYPE_ADDMSG) {
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

static void print_repeat(unsigned long type, int repeat)
{
	if (is_repeated_test(current_test) && get_assert_type(type) != PCU_TYPE_ADDMSG) {
		PCU_PRINTF1("  repeat   : %d\n", repeat);
	}
}

#define IS_ASCII(c)		(' ' == 0x20 && 0x20 <= (c) && (c) <= 0x7e)

#ifdef PCU_DEFINED_LLONG
#define PCU_LX_LD	" : 0x%0*llx (%lld)"
#define PCU_LD	" (%lld)"
#elif defined(PCU_DEFINED_WIN32_I64)
#define PCU_LX_LD	" : 0x%0*I64x (%I64d)"
#define PCU_LD	" (%I64d)"
#else
#define PCU_LX_LD	" : 0x%0*lx (%ld)"
#define PCU_LD	" (%ld)"
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
static void print_type_strw(const char *str, const wchar_t *value)
{
	if (value) {
		size_t i;
		size_t len;
		PCU_puts("  ");
		PCU_puts(str);
		PCU_puts(" : L\"");
		len = PCU_WCSLEN(value);
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

static void print_params(unsigned long type, PCU_size_t expected, PCU_size_t actual)
{
	const char * const expected_str = "expected";
	const char * const actual_str   = "actual  ";
	const char * const value1_str = "value1  ";
	const char * const value2_str = "value2  ";
	const char * s1 = expected_str;
	const char * s2 = actual_str;
	size_t len;

	if (get_not_flag(type)) {
		s1 = value1_str;
		s2 = value2_str;
	}
	switch (get_assert_type(type)) {
	case PCU_TYPE_NONE:
	case PCU_TYPE_BOOL:
		PCU_PRINTF1("  expected : %s\n", get_not_flag(type) ? "TRUE" : "FALSE");
		PCU_PRINTF1("  actual   : %s", get_not_flag(type) ? "FALSE" : "TRUE");
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
		print_expected_actual_no_div32("value1  ", "value2  ", expected, actual);
#else
		print_expected_actual("value1  ", "value2  ", expected, actual);
#endif
		break;
	case PCU_TYPE_PTR:
		print_type_ptr(s1, (const void *)(size_t) expected);
		print_type_ptr(s2  , (const void *)(size_t) actual);
		break;
	case PCU_TYPE_PTR_NULL:
		PCU_PRINTF1("  expected : %sNULL\n", get_not_flag(type) ? "non-" : "");
		print_type_ptr(actual_str, (const void *)(size_t) actual);
		break;
	case PCU_TYPE_STR:
		print_type_str(s1, (const char *)(size_t) expected);
		print_type_str(s2  , (const char *)(size_t) actual);
		break;
	case PCU_TYPE_NSTR:
		len = get_nstr_len(type);
		print_type_nstr(s1, (const char *)(size_t) expected, len);
		print_type_nstr(s2  , (const char *)(size_t) actual, len);
		PCU_PRINTF1("  length   : %u\n", (unsigned int) len);
		break;
#ifdef PCU_USE_WCHAR
	case PCU_TYPE_STRW:
		print_type_strw(s1, (const wchar_t *)(size_t) expected);
		print_type_strw(s2  , (const wchar_t *)(size_t) actual);
		break;
	case PCU_TYPE_NSTRW:
		len = get_nstr_len(type);
		print_type_nstrw(s1, (const wchar_t *)(size_t) expected, len);
		print_type_nstrw(s2  , (const wchar_t *)(size_t) actual, len);
		PCU_PRINTF1("  length   : %u\n", (unsigned int) len);
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

static void print_params_double(unsigned long type, double expected, double actual, double delta)
{
#ifndef PCU_NO_FLOATINGPOINT
	switch (get_assert_type(type)) {
	case PCU_TYPE_DBL:
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
		PCU_PRINTF1("  expected : |value1 - value2| %s |delta|\n", get_not_flag(type) ? ">" : "<=");
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

