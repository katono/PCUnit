#ifndef PCUNIT_H_INCLUDED
#define PCUNIT_H_INCLUDED

#include "PCU_Libc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * private
 */
#ifndef PCU_MESSAGE_BUF_SIZE
#define PCU_MESSAGE_BUF_SIZE	256
#endif
extern char PCU_msg_buf[PCU_MESSAGE_BUF_SIZE];

#define PCU_TYPE_NONE      0x00000000
#define PCU_TYPE_BOOL      0x00000001
#define PCU_TYPE_NUM_CHAR  0x00000002
#define PCU_TYPE_NUM_SHORT 0x00000003
#define PCU_TYPE_NUM_INT   0x00000004
#define PCU_TYPE_NUM_LONG  0x00000005
#define PCU_TYPE_NUM_SIZET 0x00000006
#define PCU_TYPE_NUM_LLONG 0x00000007
#define PCU_TYPE_OP_CHAR   0x00000008
#define PCU_TYPE_OP_SHORT  0x00000009
#define PCU_TYPE_OP_INT    0x0000000a
#define PCU_TYPE_OP_LONG   0x0000000b
#define PCU_TYPE_OP_SIZET  0x0000000c
#define PCU_TYPE_OP_LLONG  0x0000000d
#define PCU_TYPE_PTR       0x0000000e
#define PCU_TYPE_PTR_NULL  0x0000000f
#define PCU_TYPE_STR       0x00000010
#define PCU_TYPE_DBL       0x00000011
#define PCU_TYPE_MSG       0x00000012
#define PCU_TYPE_FAIL      0x00000013
#define PCU_TYPE_SETUP     0x00000014
#define PCU_TYPE_NSTR      0x40000000
#define PCU_TYPE_NOT       0x80000000

#define PCU_GET_ASSERT_TYPE(type)		(((type) & PCU_TYPE_NSTR) ? PCU_TYPE_NSTR : ((type) & ~(PCU_TYPE_NSTR | PCU_TYPE_NOT)))
#define PCU_GET_NSTR_LEN(type)			(size_t)((type) & ~(PCU_TYPE_NSTR | PCU_TYPE_NOT))
#define PCU_GET_NOT_FLAG(type)			((type) & PCU_TYPE_NOT)

unsigned long PCU_get_num_type(size_t sizeof_expected, size_t sizeof_actual, int is_operator);
int PCU_assert_impl(int passed_flag, size_t expected, size_t actual, unsigned long type, const char *expr, const char *file, unsigned int line, int fatal_flag);
#ifndef PCU_NO_FLOATINGPOINT
int PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *expr, const char *file, unsigned int line, int fatal_flag);
#endif
void PCU_msg_impl(const char *msg, const char *file, unsigned int line);


typedef struct {
	int num_asserts;
	int num_asserts_ran;
	int num_asserts_failed;
	int num_errors_setup;
	int num_errors_teardown;
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
		size_t num;
		char *str;
		const void *ptr;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		double dbl;
#endif
	} expected;
	union {
		size_t num;
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
	const char *expr;
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
#ifndef PCU_NO_CONSOLE_RUN
void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num);
#endif


/* 
 * Assert Macros
 */
#define PCU_ASSERT(expr)	PCU_assert_impl(((expr) != 0), 0, 0, PCU_TYPE_NONE, "PCU_ASSERT(" #expr ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_TRUE(expr)	PCU_assert_impl(((expr) != 0), 1, 0, PCU_TYPE_BOOL, "PCU_ASSERT_TRUE(" #expr ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_FALSE(expr)	PCU_assert_impl(((expr) == 0), 0, 1, PCU_TYPE_BOOL, "PCU_ASSERT_FALSE(" #expr ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_EQUAL(expected, actual)		PCU_assert_impl(((expected) == (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0), "PCU_ASSERT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_NOT_EQUAL(expected, actual)	PCU_assert_impl(((expected) != (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0) | PCU_TYPE_NOT, "PCU_ASSERT_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_PTR_EQUAL(expected, actual)		PCU_assert_impl(((const void *)(expected) == (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR, "PCU_ASSERT_PTR_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)	PCU_assert_impl(((const void *)(expected) != (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_PTR_NULL(value)		PCU_assert_impl(((const void *)(value) == 0), 0, (size_t)(value), PCU_TYPE_PTR_NULL, "PCU_ASSERT_PTR_NULL(" #value ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_PTR_NOT_NULL(value)	PCU_assert_impl(((const void *)(value) != 0), 1, (size_t)(value), PCU_TYPE_PTR_NULL | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_NULL(" #value ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_STRING_EQUAL(expected, actual)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) == 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR, "PCU_ASSERT_STRING_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) != 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR | PCU_TYPE_NOT, "PCU_ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)

#define PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) == 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | (n)), "PCU_ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) != 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | PCU_TYPE_NOT | (n)), "PCU_ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)		PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL, "PCU_ASSERT_DOUBLE_EQUAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)	PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL | PCU_TYPE_NOT, "PCU_ASSERT_DOUBLE_NOT_EQUAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)
#endif

#define PCU_ASSERT_OPERATOR(lhs, op, rhs)	PCU_assert_impl(((lhs) op (rhs)), (size_t)(lhs), (size_t)(rhs), PCU_get_num_type(sizeof(lhs), sizeof(rhs), 1), "PCU_ASSERT_OPERATOR((" #lhs ") " #op " (" #rhs "))", __FILE__, __LINE__, 0)


#define PCU_ASSERT_RETURN(expr)	if (!PCU_assert_impl(((expr) != 0), 0, 0, PCU_TYPE_NONE, "PCU_ASSERT_RETURN(" #expr ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_TRUE_RETURN(expr)	if (!PCU_assert_impl(((expr) != 0), 1, 0, PCU_TYPE_BOOL, "PCU_ASSERT_TRUE_RETURN(" #expr ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_FALSE_RETURN(expr)	if (!PCU_assert_impl(((expr) == 0), 0, 1, PCU_TYPE_BOOL, "PCU_ASSERT_FALSE_RETURN(" #expr ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_EQUAL_RETURN(expected, actual)		if (!PCU_assert_impl(((expected) == (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0), "PCU_ASSERT_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_NOT_EQUAL_RETURN(expected, actual)	if (!PCU_assert_impl(((expected) != (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0) | PCU_TYPE_NOT, "PCU_ASSERT_NOT_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_PTR_EQUAL_RETURN(expected, actual)		if (!PCU_assert_impl(((const void *)(expected) == (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR, "PCU_ASSERT_PTR_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_PTR_NOT_EQUAL_RETURN(expected, actual)	if (!PCU_assert_impl(((const void *)(expected) != (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_PTR_NULL_RETURN(value)		if (!PCU_assert_impl(((const void *)(value) == 0), 0, (size_t)(value), PCU_TYPE_PTR_NULL, "PCU_ASSERT_PTR_NULL_RETURN(" #value ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_PTR_NOT_NULL_RETURN(value)	if (!PCU_assert_impl(((const void *)(value) != 0), 1, (size_t)(value), PCU_TYPE_PTR_NULL | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_NULL_RETURN(" #value ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_STRING_EQUAL_RETURN(expected, actual)		if (!PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) == 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR, "PCU_ASSERT_STRING_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_STRING_NOT_EQUAL_RETURN(expected, actual)	if (!PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) != 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR | PCU_TYPE_NOT, "PCU_ASSERT_STRING_NOT_EQUAL_RETURN(" #expected ", " #actual ")", __FILE__, __LINE__, 0)) return

#define PCU_ASSERT_NSTRING_EQUAL_RETURN(expected, actual, n)		if (!PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) == 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | (n)), "PCU_ASSERT_NSTRING_EQUAL_RETURN(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(expected, actual, n)	if (!PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) != 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | PCU_TYPE_NOT | (n)), "PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)) return

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_RETURN(expected, actual, delta)		if (!PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL, "PCU_ASSERT_DOUBLE_EQUAL_RETURN(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)) return
#define PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(expected, actual, delta)	if (!PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL | PCU_TYPE_NOT, "PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)) return
#endif

#define PCU_ASSERT_OPERATOR_RETURN(lhs, op, rhs)	if (!PCU_assert_impl(((lhs) op (rhs)), (size_t)(lhs), (size_t)(rhs), PCU_get_num_type(sizeof(lhs), sizeof(rhs), 1), "PCU_ASSERT_OPERATOR_RETURN((" #lhs ") " #op " (" #rhs "))", __FILE__, __LINE__, 0)) return


#define PCU_FAIL_IMPL()		PCU_assert_impl(0, (size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL, "PCU_FAIL", __FILE__, __LINE__, 0)
#define PCU_FAIL_IMPL_R()	PCU_assert_impl(0, (size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL, "PCU_FAIL_RETURN", __FILE__, __LINE__, 0)
#define PCU_FAIL_IMPL_F()	PCU_assert_impl(0, (size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL, "PCU_FAIL_FATAL", __FILE__, __LINE__, 1)

#define PCU_FAIL0(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, sizeof PCU_msg_buf, format)                                    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL1(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, sizeof PCU_msg_buf, format, a1)                                ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL2(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2)                            ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL3(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3)                        ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL4(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4)                    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL5(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5)                ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL6(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6)            ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL7(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL8(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_FAIL_IMPL(); } while (0)

#define PCU_FAIL0_RETURN(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, sizeof PCU_msg_buf, format)                                    ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL1_RETURN(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, sizeof PCU_msg_buf, format, a1)                                ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL2_RETURN(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2)                            ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL3_RETURN(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3)                        ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL4_RETURN(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4)                    ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL5_RETURN(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5)                ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL6_RETURN(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6)            ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL7_RETURN(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL8_RETURN(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_FAIL_IMPL_R(); return; } while (0)
#define PCU_FAIL9_RETURN(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_FAIL_IMPL_R(); return; } while (0)

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_FAIL(...)			do { PCU_snprintf(PCU_msg_buf, sizeof PCU_msg_buf, __VA_ARGS__); PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL_RETURN(...)	do { PCU_snprintf(PCU_msg_buf, sizeof PCU_msg_buf, __VA_ARGS__); PCU_FAIL_IMPL_R(); return; } while (0)
#endif

#define PCU_MSG_IMPL()	PCU_msg_impl(PCU_msg_buf, __FILE__, __LINE__)

#define PCU_MESSAGE0(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, sizeof PCU_msg_buf, format)                                    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE1(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, sizeof PCU_msg_buf, format, a1)                                ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE2(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2)                            ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE3(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3)                        ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE4(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4)                    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE5(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5)                ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE6(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6)            ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE7(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE8(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MESSAGE9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_MSG_IMPL(); } while (0)

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_MESSAGE(...)	do { PCU_snprintf(PCU_msg_buf, sizeof PCU_msg_buf, __VA_ARGS__); PCU_MSG_IMPL(); } while (0)
#endif


#if !defined(PCU_NO_SETJMP) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_FATAL(expr)	PCU_assert_impl(((expr) != 0), 0, 0, PCU_TYPE_NONE, "PCU_ASSERT_FATAL(" #expr ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_TRUE_FATAL(expr)		PCU_assert_impl(((expr) != 0), 1, 0, PCU_TYPE_BOOL, "PCU_ASSERT_TRUE_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_FALSE_FATAL(expr)	PCU_assert_impl(((expr) == 0), 0, 1, PCU_TYPE_BOOL, "PCU_ASSERT_FALSE_FATAL(" #expr ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_EQUAL_FATAL(expected, actual)		PCU_assert_impl(((expected) == (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0), "PCU_ASSERT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl(((expected) != (actual)), (size_t)(expected), (size_t)(actual), PCU_get_num_type(sizeof(expected), sizeof(actual), 0) | PCU_TYPE_NOT, "PCU_ASSERT_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_PTR_EQUAL_FATAL(expected, actual)		PCU_assert_impl(((const void *)(expected) == (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR, "PCU_ASSERT_PTR_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl(((const void *)(expected) != (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_PTR_NULL_FATAL(value)		PCU_assert_impl(((const void *)(value) == 0), 0, (size_t)(value), PCU_TYPE_PTR_NULL, "PCU_ASSERT_PTR_NULL_FATAL(" #value ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_PTR_NOT_NULL_FATAL(value)	PCU_assert_impl(((const void *)(value) != 0), 1, (size_t)(value), PCU_TYPE_PTR_NULL | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_NULL_FATAL(" #value ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_STRING_EQUAL_FATAL(expected, actual)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) == 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR, "PCU_ASSERT_STRING_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) != 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR | PCU_TYPE_NOT, "PCU_ASSERT_STRING_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) == 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | (n)), "PCU_ASSERT_NSTRING_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) != 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | PCU_TYPE_NOT | (n)), "PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_FATAL(expected, actual, delta)		PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL, "PCU_ASSERT_DOUBLE_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(expected, actual, delta)	PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL | PCU_TYPE_NOT, "PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 1)
#endif

#define PCU_ASSERT_OPERATOR_FATAL(lhs, op, rhs)	PCU_assert_impl(((lhs) op (rhs)), (size_t)(lhs), (size_t)(rhs), PCU_get_num_type(sizeof(lhs), sizeof(rhs), 1), "PCU_ASSERT_OPERATOR_FATAL((" #lhs ") " #op " (" #rhs "))", __FILE__, __LINE__, 1)

#define PCU_FAIL0_FATAL(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, sizeof PCU_msg_buf, format)                                    ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL1_FATAL(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, sizeof PCU_msg_buf, format, a1)                                ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL2_FATAL(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2)                            ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL3_FATAL(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3)                        ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL4_FATAL(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4)                    ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL5_FATAL(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5)                ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL6_FATAL(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6)            ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL7_FATAL(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL8_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_FAIL_IMPL_F(); } while (0)
#define PCU_FAIL9_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, sizeof PCU_msg_buf, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_FAIL_IMPL_F(); } while (0)

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_FAIL_FATAL(...)	do { PCU_snprintf(PCU_msg_buf, sizeof PCU_msg_buf, __VA_ARGS__); PCU_FAIL_IMPL_F(); } while (0)
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif /* PCUNIT_H_INCLUDED */
