#ifndef PCUNIT_H_INCLUDED
#define PCUNIT_H_INCLUDED

#include "PCU_Libc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCU_VERSION "1.4.1"

/* 
 * private
 */
extern char * const PCU_msg_buf;
extern const size_t PCU_msg_buf_size;

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
typedef unsigned long long PCU_size_t;
typedef long long PCU_ssize_t;
#else
typedef size_t PCU_size_t;
typedef ptrdiff_t PCU_ssize_t;
#endif

#define PCU_TYPE_NONE      0x00000000
#define PCU_TYPE_BOOL      0x00000001
#define PCU_TYPE_NUM       0x00000002
#define PCU_TYPE_PTR       0x00000003
#define PCU_TYPE_PTR_NULL  0x00000004
#define PCU_TYPE_STR       0x00000005
#define PCU_TYPE_STRW      0x00000006
#define PCU_TYPE_DBL       0x00000007
#define PCU_TYPE_OP        0x00000008
#define PCU_TYPE_OP_INT    0x00000009
#define PCU_TYPE_OP_DBL    0x0000000a
#define PCU_TYPE_MSG       0x0000000b
#define PCU_TYPE_FAIL      0x0000000c
#define PCU_TYPE_MSGW      0x0000000d
#define PCU_TYPE_FAILW     0x0000000e
#define PCU_TYPE_SETUP     0x0000000f
#define PCU_TYPE_NSTR      0x20000000
#define PCU_TYPE_NSTRW     0x40000000
#define PCU_TYPE_NOT       0x80000000

unsigned long PCU_get_assert_type(unsigned long type);
size_t PCU_get_nstr_len(unsigned long type);
int PCU_get_not_flag(unsigned long type);
void PCU_assert_impl(int passed_flag, PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
void PCU_assert_num_impl(PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
void PCU_assert_ptr_impl(const void *expected, const void *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
void PCU_assert_str_impl(const char *expected, const char *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
void PCU_assert_strw_impl(const wchar_t *expected, const wchar_t *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
#endif
#ifndef PCU_NO_FLOATINGPOINT
void PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
void PCU_assert_op_double_impl(int passed_flag, double expected, double actual, unsigned long type, const char *str_assert, const char *file, unsigned int line, int fatal_flag);
#endif
void PCU_msg_impl(const char *msg, unsigned long type, const char *str_assert, const char *file, unsigned int line);


typedef struct {
	int num_asserts;
	int num_asserts_ran;
	int num_asserts_failed;
	int num_errors_setup;
	int num_errors_teardown;
	int num_msgs;
} PCU_TestResult;

typedef struct {
	int num_tests;
	int num_tests_ran;
	int num_tests_failed;
	int num_errors_initialize;
	int num_errors_cleanup;
	PCU_TestResult test_result;
} PCU_SuiteResult;

typedef struct {
	int num_suites;
	int num_suites_ran;
	int num_suites_failed;
	PCU_SuiteResult suite_result;
} PCU_Result;

typedef struct PCU_TestFailure {
	union {
		PCU_size_t num;
		char *str;
		const void *ptr;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		double dbl;
#endif
	} expected;
	union {
		PCU_size_t num;
		char *str;
		const void *ptr;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		double dbl;
#endif
	} actual;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
	double delta;
#endif
	unsigned long type;
	const char *str_assert;
	const char *file;
	unsigned int line;
	int repeat;
	struct PCU_TestFailure *next;
	struct PCU_TestFailure *prev;
} PCU_TestFailure;

/* 
 * public
 */
typedef struct PCU_Test PCU_Test;
typedef struct PCU_Suite PCU_Suite;

struct PCU_Test {
	/* public */
	const char *name;
	void (*test)(void);
	int ntimes;
	/* private */
	const PCU_Suite *suite;
	PCU_TestResult result;
	PCU_TestFailure failure_list;
};

struct PCU_Suite {
	/* public */
	const char *name;
	PCU_Test *tests;
	int ntests;
	int (*setup)(void);
	int (*teardown)(void);
	int (*initialize)(void);
	int (*cleanup)(void);
	/* private */
	int initialize_error;
	int cleanup_error;
	PCU_SuiteResult result;
};

typedef PCU_Suite *(*PCU_SuiteMethod)(void);

typedef int (*PCU_Putchar)(int c);
typedef int (*PCU_Getchar)(void);

/* 
 * PCUnit API
 */
void PCU_run(const PCU_SuiteMethod *suite_methods, int num);
void PCU_set_putchar(PCU_Putchar func);
void PCU_set_getchar(PCU_Getchar func);
int PCU_repeat_counter(void);
const char *PCU_test_name(void);
const char *PCU_suite_name(void);
void PCU_enable_color(void);
void PCU_disable_color(void);
int PCU_last_assertion(void);
#ifndef PCU_NO_CONSOLE_RUN
void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num);
#endif


/* 
 * Assert Macros
 */

#define PCU_ASSERT_AUX(expr, str_assert, fatal_flag)\
	PCU_assert_impl(((expr) != 0),\
		0, 0,\
		PCU_TYPE_NONE,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_TRUE_AUX(expr, str_assert, fatal_flag)\
	PCU_assert_impl(((expr) != 0),\
		0, 0,\
		PCU_TYPE_BOOL,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_FALSE_AUX(expr, str_assert, fatal_flag)\
	PCU_assert_impl(((expr) == 0),\
		0, 0,\
		PCU_TYPE_BOOL,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_num_impl((PCU_size_t) (expected), (PCU_size_t) (actual),\
		PCU_TYPE_NUM,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_NOT_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_num_impl((PCU_size_t) (expected), (PCU_size_t) (actual),\
		PCU_TYPE_NUM | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_PTR_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_ptr_impl((const void *) (expected), (const void *) (actual),\
		PCU_TYPE_PTR,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_ptr_impl((const void *) (expected), (const void *) (actual),\
		PCU_TYPE_PTR | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_PTR_NULL_AUX(value, str_assert, fatal_flag)\
	PCU_assert_ptr_impl(0, (const void *) (value),\
		PCU_TYPE_PTR_NULL,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_PTR_NOT_NULL_AUX(value, str_assert, fatal_flag)\
	PCU_assert_ptr_impl(0, (const void *) (value),\
		PCU_TYPE_PTR_NULL | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_STRING_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_STR,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_STR | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n, str_assert, fatal_flag)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_NSTR | (n),\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n, str_assert, fatal_flag)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_NSTR | PCU_TYPE_NOT | (n),\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_STRW,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual, str_assert, fatal_flag)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_STRW | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n, str_assert, fatal_flag)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_NSTRW | (n),\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n, str_assert, fatal_flag)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_NSTRW | PCU_TYPE_NOT | (n),\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta, str_assert, fatal_flag)\
	PCU_assert_double_impl((expected), (actual), (delta),\
		PCU_TYPE_DBL,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta, str_assert, fatal_flag)\
	PCU_assert_double_impl((expected), (actual), (delta),\
		PCU_TYPE_DBL | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__, fatal_flag)

#define PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs, str_assert, fatal_flag)\
	do {\
		const double pcu_assert_operator_aux_lhs = (double) (lhs);\
		const double pcu_assert_operator_aux_rhs = (double) (rhs);\
		PCU_assert_op_double_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			pcu_assert_operator_aux_lhs, pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP_DBL,\
			str_assert,\
			__FILE__, __LINE__, fatal_flag);\
	} while (0)
#endif

#define PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs, str_assert, fatal_flag)\
	do {\
		const PCU_size_t pcu_assert_operator_aux_lhs = (PCU_size_t) (lhs);\
		const PCU_size_t pcu_assert_operator_aux_rhs = (PCU_size_t) (rhs);\
		PCU_assert_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			pcu_assert_operator_aux_lhs, pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP,\
			str_assert,\
			__FILE__, __LINE__, fatal_flag);\
	} while (0)

#define PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs, str_assert, fatal_flag)\
	do {\
		const PCU_ssize_t pcu_assert_operator_aux_lhs = (PCU_ssize_t) (lhs);\
		const PCU_ssize_t pcu_assert_operator_aux_rhs = (PCU_ssize_t) (rhs);\
		PCU_assert_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			(PCU_size_t) pcu_assert_operator_aux_lhs, (PCU_size_t) pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP_INT,\
			str_assert,\
			__FILE__, __LINE__, fatal_flag);\
	} while (0)


#define PCU_ASSERT(expr)\
	PCU_ASSERT_AUX(expr,\
		"PCU_ASSERT(" #expr ")",\
		0)

#define PCU_ASSERT_TRUE(expr)\
	PCU_ASSERT_TRUE_AUX(expr,\
		"PCU_ASSERT_TRUE(" #expr ")",\
		0)

#define PCU_ASSERT_FALSE(expr)\
	PCU_ASSERT_FALSE_AUX(expr,\
		"PCU_ASSERT_FALSE(" #expr ")",\
		0)

#define PCU_ASSERT_EQUAL(expected, actual)\
	PCU_ASSERT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_NOT_EQUAL(expected, actual)\
	PCU_ASSERT_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_NOT_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_PTR_EQUAL(expected, actual)\
	PCU_ASSERT_PTR_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_PTR_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)\
	PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_PTR_NULL(value)\
	PCU_ASSERT_PTR_NULL_AUX(value,\
		"PCU_ASSERT_PTR_NULL(" #value ")",\
		0)

#define PCU_ASSERT_PTR_NOT_NULL(value)\
	PCU_ASSERT_PTR_NOT_NULL_AUX(value,\
		"PCU_ASSERT_PTR_NOT_NULL(" #value ")",\
		0)

#define PCU_ASSERT_STRING_EQUAL(expected, actual)\
	PCU_ASSERT_STRING_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRING_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)\
	PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)\
	PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")",\
		0)

#define PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)\
	PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")",\
		0)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL(expected, actual)\
	PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRINGW_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_STRINGW_NOT_EQUAL(expected, actual)\
	PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRINGW_NOT_EQUAL(" #expected ", " #actual ")",\
		0)

#define PCU_ASSERT_NSTRINGW_EQUAL(expected, actual, n)\
	PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRINGW_EQUAL(" #expected ", " #actual ", " #n ")",\
		0)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL(expected, actual, n)\
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRINGW_NOT_EQUAL(" #expected ", " #actual ", " #n ")",\
		0)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)\
	PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta,\
		"PCU_ASSERT_DOUBLE_EQUAL(" #expected ", " #actual ", " #delta ")",\
		0)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)\
	PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta,\
		"PCU_ASSERT_DOUBLE_NOT_EQUAL(" #expected ", " #actual ", " #delta ")",\
		0)

#define PCU_ASSERT_OPERATOR_DOUBLE(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR_DOUBLE((" #lhs ") " #op " (" #rhs "))",\
		0)
#endif

#define PCU_ASSERT_OPERATOR(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR((" #lhs ") " #op " (" #rhs "))",\
		0)

#define PCU_ASSERT_OPERATOR_INT(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR_INT((" #lhs ") " #op " (" #rhs "))",\
		0)


#define PCU_ASSERT_RETURN(expr)\
	do {\
		PCU_ASSERT_AUX(expr,\
			"PCU_ASSERT_RETURN(" #expr ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_TRUE_RETURN(expr)\
	do {\
		PCU_ASSERT_TRUE_AUX(expr,\
			"PCU_ASSERT_TRUE_RETURN(" #expr ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_FALSE_RETURN(expr)\
	do {\
		PCU_ASSERT_FALSE_AUX(expr,\
			"PCU_ASSERT_FALSE_RETURN(" #expr ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_NOT_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_NOT_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_PTR_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_PTR_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_PTR_NOT_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_NOT_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_PTR_NULL_RETURN(value)\
	do {\
		PCU_ASSERT_PTR_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NULL_RETURN(" #value ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_PTR_NOT_NULL_RETURN(value)\
	do {\
		PCU_ASSERT_PTR_NOT_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NOT_NULL_RETURN(" #value ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_STRING_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_STRING_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_STRING_NOT_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_NOT_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_NSTRING_EQUAL_RETURN(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_EQUAL_RETURN(" #expected ", " #actual ", " #n ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(" #expected ", " #actual ", " #n ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_STRINGW_NOT_EQUAL_RETURN(expected, actual)\
	do {\
		PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_NOT_EQUAL_RETURN(" #expected ", " #actual ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_NSTRINGW_EQUAL_RETURN(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_EQUAL_RETURN(" #expected ", " #actual ", " #n ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL_RETURN(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_NOT_EQUAL_RETURN(" #expected ", " #actual ", " #n ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_RETURN(expected, actual, delta)\
	do {\
		PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_EQUAL_RETURN(" #expected ", " #actual ", " #delta ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(expected, actual, delta)\
	do {\
		PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(" #expected ", " #actual ", " #delta ")",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_OPERATOR_DOUBLE_RETURN(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_DOUBLE_RETURN((" #lhs ") " #op " (" #rhs "))",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)
#endif

#define PCU_ASSERT_OPERATOR_RETURN(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_RETURN((" #lhs ") " #op " (" #rhs "))",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)

#define PCU_ASSERT_OPERATOR_INT_RETURN(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_INT_RETURN((" #lhs ") " #op " (" #rhs "))",\
			0);\
		if (!PCU_last_assertion()) return;\
	} while (0)


#define PCU_FAIL_IMPL()    PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL , "PCU_FAIL"        , __FILE__, __LINE__, 0)
#define PCU_FAIL_IMPL_R()  PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL , "PCU_FAIL_RETURN" , __FILE__, __LINE__, 0)
#define PCU_FAIL_IMPL_F()  PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL , "PCU_FAIL_FATAL"  , __FILE__, __LINE__, 1)
#define PCU_FAILW_IMPL()   PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAILW, "PCU_FAILW"       , __FILE__, __LINE__, 0)
#define PCU_FAILW_IMPL_R() PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAILW, "PCU_FAILW_RETURN", __FILE__, __LINE__, 0)
#define PCU_FAILW_IMPL_F() PCU_assert_impl(0, (PCU_size_t)(size_t)(PCU_msg_buf), 0, PCU_TYPE_FAILW, "PCU_FAILW_FATAL" , __FILE__, __LINE__, 1)

#define PCU_FAIL0(format)\
	do {\
		PCU_SNPRINTF0(PCU_msg_buf, PCU_msg_buf_size, format);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL1(format, a1)\
	do {\
		PCU_SNPRINTF1(PCU_msg_buf, PCU_msg_buf_size, format, a1);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL2(format, a1, a2)\
	do {\
		PCU_SNPRINTF2(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL3(format, a1, a2, a3)\
	do {\
		PCU_SNPRINTF3(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL4(format, a1, a2, a3, a4)\
	do {\
		PCU_SNPRINTF4(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL5(format, a1, a2, a3, a4, a5)\
	do {\
		PCU_SNPRINTF5(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL6(format, a1, a2, a3, a4, a5, a6)\
	do {\
		PCU_SNPRINTF6(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL7(format, a1, a2, a3, a4, a5, a6, a7)\
	do {\
		PCU_SNPRINTF7(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL8(format, a1, a2, a3, a4, a5, a6, a7, a8)\
	do {\
		PCU_SNPRINTF8(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9)\
	do {\
		PCU_SNPRINTF9(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
		PCU_FAIL_IMPL();\
	} while (0)


#define PCU_FAIL0_RETURN(format)\
	do {\
		PCU_SNPRINTF0(PCU_msg_buf, PCU_msg_buf_size, format);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL1_RETURN(format, a1)\
	do {\
		PCU_SNPRINTF1(PCU_msg_buf, PCU_msg_buf_size, format, a1);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL2_RETURN(format, a1, a2)\
	do {\
		PCU_SNPRINTF2(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL3_RETURN(format, a1, a2, a3)\
	do {\
		PCU_SNPRINTF3(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL4_RETURN(format, a1, a2, a3, a4)\
	do {\
		PCU_SNPRINTF4(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL5_RETURN(format, a1, a2, a3, a4, a5)\
	do {\
		PCU_SNPRINTF5(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL6_RETURN(format, a1, a2, a3, a4, a5, a6)\
	do {\
		PCU_SNPRINTF6(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL7_RETURN(format, a1, a2, a3, a4, a5, a6, a7)\
	do {\
		PCU_SNPRINTF7(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL8_RETURN(format, a1, a2, a3, a4, a5, a6, a7, a8)\
	do {\
		PCU_SNPRINTF8(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)

#define PCU_FAIL9_RETURN(format, a1, a2, a3, a4, a5, a6, a7, a8, a9)\
	do {\
		PCU_SNPRINTF9(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)


#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_FAIL(...)\
	do {\
		PCU_snprintf(PCU_msg_buf, PCU_msg_buf_size, __VA_ARGS__);\
		PCU_FAIL_IMPL();\
	} while (0)

#define PCU_FAIL_RETURN(...)\
	do {\
		PCU_snprintf(PCU_msg_buf, PCU_msg_buf_size, __VA_ARGS__);\
		PCU_FAIL_IMPL_R(); return;\
	} while (0)
#if !defined(PCU_NO_WCHAR)
#define PCU_FAILW(...)\
	do {\
		PCU_SWPRINTF((wchar_t *) PCU_msg_buf, PCU_msg_buf_size / sizeof(wchar_t), __VA_ARGS__);\
		PCU_FAILW_IMPL();\
	} while (0)

#define PCU_FAILW_RETURN(...)\
	do {\
		PCU_SWPRINTF((wchar_t *) PCU_msg_buf, PCU_msg_buf_size / sizeof(wchar_t), __VA_ARGS__);\
		PCU_FAILW_IMPL_R(); return;\
	} while (0)
#endif
#endif

#define PCU_MSG_IMPL()  PCU_msg_impl(PCU_msg_buf, PCU_TYPE_MSG , "PCU_MESSAGE" , __FILE__, __LINE__)
#define PCU_MSGW_IMPL() PCU_msg_impl(PCU_msg_buf, PCU_TYPE_MSGW, "PCU_MESSAGEW", __FILE__, __LINE__)

#define PCU_MESSAGE0(format)\
	do {\
		PCU_SNPRINTF0(PCU_msg_buf, PCU_msg_buf_size, format);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE1(format, a1)\
	do {\
		PCU_SNPRINTF1(PCU_msg_buf, PCU_msg_buf_size, format, a1);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE2(format, a1, a2)\
	do {\
		PCU_SNPRINTF2(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE3(format, a1, a2, a3)\
	do {\
		PCU_SNPRINTF3(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE4(format, a1, a2, a3, a4)\
	do {\
		PCU_SNPRINTF4(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE5(format, a1, a2, a3, a4, a5)\
	do {\
		PCU_SNPRINTF5(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE6(format, a1, a2, a3, a4, a5, a6)\
	do {\
		PCU_SNPRINTF6(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE7(format, a1, a2, a3, a4, a5, a6, a7)\
	do {\
		PCU_SNPRINTF7(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE8(format, a1, a2, a3, a4, a5, a6, a7, a8)\
	do {\
		PCU_SNPRINTF8(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8);\
		PCU_MSG_IMPL();\
	} while (0)

#define PCU_MESSAGE9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9)\
	do {\
		PCU_SNPRINTF9(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
		PCU_MSG_IMPL();\
	} while (0)


#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_MESSAGE(...)\
	do {\
		PCU_snprintf(PCU_msg_buf, PCU_msg_buf_size, __VA_ARGS__);\
		PCU_MSG_IMPL();\
	} while (0)
#if !defined(PCU_NO_WCHAR)
#define PCU_MESSAGEW(...)\
	do {\
		PCU_SWPRINTF((wchar_t *) PCU_msg_buf, PCU_msg_buf_size / sizeof(wchar_t), __VA_ARGS__);\
		PCU_MSGW_IMPL();\
	} while (0)
#endif
#endif


#if !defined(PCU_NO_SETJMP) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_FATAL(expr)\
	PCU_ASSERT_AUX(expr,\
		"PCU_ASSERT_FATAL(" #expr ")",\
		1)

#define PCU_ASSERT_TRUE_FATAL(expr)\
	PCU_ASSERT_TRUE_AUX(expr,\
		"PCU_ASSERT_TRUE_FATAL(" #expr ")",\
		1)

#define PCU_ASSERT_FALSE_FATAL(expr)\
	PCU_ASSERT_FALSE_AUX(expr,\
		"PCU_ASSERT_FALSE_FATAL(" #expr ")",\
		1)

#define PCU_ASSERT_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_NOT_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_NOT_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_PTR_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_PTR_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_PTR_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_PTR_NOT_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_PTR_NULL_FATAL(value)\
	PCU_ASSERT_PTR_NULL_AUX(value,\
		"PCU_ASSERT_PTR_NULL_FATAL(" #value ")",\
		1)

#define PCU_ASSERT_PTR_NOT_NULL_FATAL(value)\
	PCU_ASSERT_PTR_NOT_NULL_AUX(value,\
		"PCU_ASSERT_PTR_NOT_NULL_FATAL(" #value ")",\
		1)

#define PCU_ASSERT_STRING_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_STRING_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRING_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRING_NOT_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)\
	PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRING_EQUAL_FATAL(" #expected ", " #actual ", " #n ")",\
		1)

#define PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)\
	PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #n ")",\
		1)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRINGW_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_STRINGW_NOT_EQUAL_FATAL(expected, actual)\
	PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual,\
		"PCU_ASSERT_STRINGW_NOT_EQUAL_FATAL(" #expected ", " #actual ")",\
		1)

#define PCU_ASSERT_NSTRINGW_EQUAL_FATAL(expected, actual, n)\
	PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRINGW_EQUAL_FATAL(" #expected ", " #actual ", " #n ")",\
		1)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL_FATAL(expected, actual, n)\
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n,\
		"PCU_ASSERT_NSTRINGW_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #n ")",\
		1)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_FATAL(expected, actual, delta)\
	PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta,\
		"PCU_ASSERT_DOUBLE_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")",\
		1)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(expected, actual, delta)\
	PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta,\
		"PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")",\
		1)

#define PCU_ASSERT_OPERATOR_DOUBLE_FATAL(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR_DOUBLE_FATAL((" #lhs ") " #op " (" #rhs "))",\
		1)
#endif

#define PCU_ASSERT_OPERATOR_FATAL(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR_FATAL((" #lhs ") " #op " (" #rhs "))",\
		1)

#define PCU_ASSERT_OPERATOR_INT_FATAL(lhs, op, rhs)\
	PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs,\
		"PCU_ASSERT_OPERATOR_INT_FATAL((" #lhs ") " #op " (" #rhs "))",\
		1)


#define PCU_FAIL0_FATAL(format)\
	do {\
		PCU_SNPRINTF0(PCU_msg_buf, PCU_msg_buf_size, format);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL1_FATAL(format, a1)\
	do {\
		PCU_SNPRINTF1(PCU_msg_buf, PCU_msg_buf_size, format, a1);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL2_FATAL(format, a1, a2)\
	do {\
		PCU_SNPRINTF2(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL3_FATAL(format, a1, a2, a3)\
	do {\
		PCU_SNPRINTF3(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL4_FATAL(format, a1, a2, a3, a4)\
	do {\
		PCU_SNPRINTF4(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL5_FATAL(format, a1, a2, a3, a4, a5)\
	do {\
		PCU_SNPRINTF5(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL6_FATAL(format, a1, a2, a3, a4, a5, a6)\
	do {\
		PCU_SNPRINTF6(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL7_FATAL(format, a1, a2, a3, a4, a5, a6, a7)\
	do {\
		PCU_SNPRINTF7(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL8_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8)\
	do {\
		PCU_SNPRINTF8(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8);\
		PCU_FAIL_IMPL_F();\
	} while (0)

#define PCU_FAIL9_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8, a9)\
	do {\
		PCU_SNPRINTF9(PCU_msg_buf, PCU_msg_buf_size, format, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
		PCU_FAIL_IMPL_F();\
	} while (0)


#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_FAIL_FATAL(...)\
	do {\
		PCU_snprintf(PCU_msg_buf, PCU_msg_buf_size, __VA_ARGS__);\
		PCU_FAIL_IMPL_F();\
	} while (0)
#if !defined(PCU_NO_WCHAR)
#define PCU_FAILW_FATAL(...)\
	do {\
		PCU_SWPRINTF((wchar_t *) PCU_msg_buf, PCU_msg_buf_size / sizeof(wchar_t), __VA_ARGS__);\
		PCU_FAILW_IMPL_F();\
	} while (0)
#endif
#endif
#endif


#if (defined(_UNICODE) || defined(UNICODE)) && !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGT_EQUAL              PCU_ASSERT_STRINGW_EQUAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL          PCU_ASSERT_STRINGW_NOT_EQUAL
#define PCU_ASSERT_NSTRINGT_EQUAL             PCU_ASSERT_NSTRINGW_EQUAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL         PCU_ASSERT_NSTRINGW_NOT_EQUAL
#define PCU_ASSERT_STRINGT_EQUAL_RETURN       PCU_ASSERT_STRINGW_EQUAL_RETURN
#define PCU_ASSERT_STRINGT_NOT_EQUAL_RETURN   PCU_ASSERT_STRINGW_NOT_EQUAL_RETURN
#define PCU_ASSERT_NSTRINGT_EQUAL_RETURN      PCU_ASSERT_NSTRINGW_EQUAL_RETURN
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL_RETURN  PCU_ASSERT_NSTRINGW_NOT_EQUAL_RETURN
#define PCU_ASSERT_STRINGT_EQUAL_FATAL        PCU_ASSERT_STRINGW_EQUAL_FATAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL_FATAL    PCU_ASSERT_STRINGW_NOT_EQUAL_FATAL
#define PCU_ASSERT_NSTRINGT_EQUAL_FATAL       PCU_ASSERT_NSTRINGW_EQUAL_FATAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL_FATAL   PCU_ASSERT_NSTRINGW_NOT_EQUAL_FATAL
#else
#define PCU_ASSERT_STRINGT_EQUAL              PCU_ASSERT_STRING_EQUAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL          PCU_ASSERT_STRING_NOT_EQUAL
#define PCU_ASSERT_NSTRINGT_EQUAL             PCU_ASSERT_NSTRING_EQUAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL         PCU_ASSERT_NSTRING_NOT_EQUAL
#define PCU_ASSERT_STRINGT_EQUAL_RETURN       PCU_ASSERT_STRING_EQUAL_RETURN
#define PCU_ASSERT_STRINGT_NOT_EQUAL_RETURN   PCU_ASSERT_STRING_NOT_EQUAL_RETURN
#define PCU_ASSERT_NSTRINGT_EQUAL_RETURN      PCU_ASSERT_NSTRING_EQUAL_RETURN
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL_RETURN  PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN
#define PCU_ASSERT_STRINGT_EQUAL_FATAL        PCU_ASSERT_STRING_EQUAL_FATAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL_FATAL    PCU_ASSERT_STRING_NOT_EQUAL_FATAL
#define PCU_ASSERT_NSTRINGT_EQUAL_FATAL       PCU_ASSERT_NSTRING_EQUAL_FATAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL_FATAL   PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL
#endif

#if (defined(_UNICODE) || defined(UNICODE)) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_MESSAGET      PCU_MESSAGEW
#define PCU_FAILT         PCU_FAILW
#define PCU_FAILT_FATAL   PCU_FAILW_FATAL
#define PCU_FAILT_RETURN  PCU_FAILW_RETURN
#else
#define PCU_MESSAGET      PCU_MESSAGE
#define PCU_FAILT         PCU_FAIL
#define PCU_FAILT_FATAL   PCU_FAIL_FATAL
#define PCU_FAILT_RETURN  PCU_FAIL_RETURN
#endif


/* 
 * Helper Macros
 */
#define PCU_TEST(func)                  { #func, func }
#define PCU_TEST_REPEATED(func, ntimes) { #func, func, ntimes }
#define PCU_TEST_SKIPPED(func)          { #func, func, -1 }

#ifdef __cplusplus
}
#endif

#endif /* PCUNIT_H_INCLUDED */
