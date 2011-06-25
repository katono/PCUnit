#ifndef PCUNIT_H_INCLUDED
#define PCUNIT_H_INCLUDED

#include "PCU_Libc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * private
 */
#ifndef PCU_MSG_BUF_SIZE
#define PCU_MSG_BUF_SIZE	256
#endif
extern char PCU_msg_buf[];

#define PCU_TYPE_NONE	0x00000000
#define PCU_TYPE_BOOL	0x00000001
#define PCU_TYPE_NUM	0x00000002
#define PCU_TYPE_PTR	0x00000004
#define PCU_TYPE_STR	0x00000008
#define PCU_TYPE_MSG	0x00000010
#define PCU_TYPE_FAIL	0x00000011
#define PCU_TYPE_SETUP	0x00000012
#define PCU_TYPE_DBL	0x00000014
#define PCU_TYPE_LLONG	0x00000018
#define PCU_TYPE_NSTR	0x40000000
#define PCU_TYPE_NOT	0x80000000

#define PCU_GET_ASSERT_TYPE(type)		(((type) & PCU_TYPE_NSTR) ? PCU_TYPE_NSTR : ((type) & ~(PCU_TYPE_NSTR | PCU_TYPE_NOT)))
#define PCU_GET_NSTR_LEN(type)			(size_t)((type) & ~(PCU_TYPE_NSTR | PCU_TYPE_NOT))
#define PCU_GET_NOT_FLAG(type)			((type) & PCU_TYPE_NOT)

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
	int line;
	int repeat;
	struct PCU_TestFailure *next;
	struct PCU_TestFailure *prev;
} PCU_TestFailure;

/* 
 * public
 */
#define PCU_NULL	{0}

typedef struct {
	/* public */
	const char *name;
	void (*test)(void);
	int (*setup)(void);
	int (*teardown)(void);
	int repeat;
	/* private */
	PCU_TestResult result;
	PCU_TestFailure assertion_list;
} PCU_Test;

typedef struct {
	/* public */
	const char *name;
	PCU_Test *tests;
	int (*initialize)(void);
	int (*cleanup)(void);
	/* private */
	int initialize_error;
	int cleanup_error;
	PCU_SuiteResult result;
} PCU_Suite;

typedef int (*PCU_Putchar)(int c);
typedef int (*PCU_Getchar)(void);

typedef enum PCU_Mode {
	PCU_MODE_NORMAL,
	PCU_MODE_INTERACTIVE
} PCU_Mode;

/* 
 * PCUnit API
 */
void PCU_run(PCU_Suite *suite);
void PCU_set_mode(PCU_Mode mode);
void PCU_set_putchar(PCU_Putchar func);
void PCU_set_getchar(PCU_Getchar func);
int PCU_repeat_counter(void);
const char *PCU_test_name(void);
const char *PCU_suite_name(void);
void PCU_enable_color(void);
void PCU_disable_color(void);


/* 
 * Assert Macros
 */
#define PCU_ASSERT(expr)		PCU_assert_impl(((expr) != 0), 0, 0, PCU_TYPE_NONE, "PCU_ASSERT(" #expr ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_FATAL(expr)	PCU_assert_impl(((expr) != 0), 0, 0, PCU_TYPE_NONE, "PCU_ASSERT_FATAL(" #expr ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_TRUE(expr)			PCU_assert_impl(((expr) != 0), 1, 0, PCU_TYPE_BOOL, "PCU_ASSERT_TRUE(" #expr ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_TRUE_FATAL(expr)		PCU_assert_impl(((expr) != 0), 1, 0, PCU_TYPE_BOOL, "PCU_ASSERT_TRUE_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_FALSE(expr)			PCU_assert_impl(((expr) == 0), 0, 1, PCU_TYPE_BOOL, "PCU_ASSERT_FALSE(" #expr ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_FALSE_FATAL(expr)	PCU_assert_impl(((expr) == 0), 0, 1, PCU_TYPE_BOOL, "PCU_ASSERT_FALSE_FATAL(" #expr ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_EQUAL(expected, actual)				PCU_assert_impl(((expected) == (actual)), (size_t)(expected), (size_t)(actual), (sizeof(expected) > sizeof(size_t) || sizeof(actual) > sizeof(size_t)) ? PCU_TYPE_LLONG : PCU_TYPE_NUM, "PCU_ASSERT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_NOT_EQUAL(expected, actual)			PCU_assert_impl(((expected) != (actual)), (size_t)(expected), (size_t)(actual), (sizeof(expected) > sizeof(size_t) || sizeof(actual) > sizeof(size_t)) ? PCU_TYPE_LLONG : PCU_TYPE_NUM | PCU_TYPE_NOT, "PCU_ASSERT_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_EQUAL_FATAL(expected, actual)		PCU_assert_impl(((expected) == (actual)), (size_t)(expected), (size_t)(actual), (sizeof(expected) > sizeof(size_t) || sizeof(actual) > sizeof(size_t)) ? PCU_TYPE_LLONG : PCU_TYPE_NUM, "PCU_ASSERT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl(((expected) != (actual)), (size_t)(expected), (size_t)(actual), (sizeof(expected) > sizeof(size_t) || sizeof(actual) > sizeof(size_t)) ? PCU_TYPE_LLONG : PCU_TYPE_NUM | PCU_TYPE_NOT, "PCU_ASSERT_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_PTR_EQUAL(expected, actual)				PCU_assert_impl(((const void *)(expected) == (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR, "PCU_ASSERT_PTR_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)			PCU_assert_impl(((const void *)(expected) != (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_PTR_EQUAL_FATAL(expected, actual)		PCU_assert_impl(((const void *)(expected) == (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR, "PCU_ASSERT_PTR_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl(((const void *)(expected) != (const void *)(actual)), (size_t)(expected), (size_t)(actual), PCU_TYPE_PTR | PCU_TYPE_NOT, "PCU_ASSERT_PTR_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_STRING_EQUAL(expected, actual)			PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) == 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR, "PCU_ASSERT_STRING_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) != 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR | PCU_TYPE_NOT, "PCU_ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_STRING_EQUAL_FATAL(expected, actual)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) == 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR, "PCU_ASSERT_STRING_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRCMP((expected), (actual)) != 0) : 0), (size_t)(expected), (size_t)(actual), PCU_TYPE_STR | PCU_TYPE_NOT, "PCU_ASSERT_STRING_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)			PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) == 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | (n)), "PCU_ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) != 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | PCU_TYPE_NOT | (n)), "PCU_ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)		PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) == 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | (n)), "PCU_ASSERT_NSTRING_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)	PCU_assert_impl((((size_t)(expected) != 0 && (size_t)(actual) != 0) ? (PCU_STRNCMP((expected), (actual), (n)) != 0) : 0), (size_t)(expected), (size_t)(actual), (PCU_TYPE_NSTR | PCU_TYPE_NOT | (n)), "PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)			PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL, "PCU_ASSERT_DOUBLE_EQUAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)		PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL | PCU_TYPE_NOT, "PCU_ASSERT_DOUBLE_NOT_EQUAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 0)
#define PCU_ASSERT_DOUBLE_EQUAL_FATAL(expected, actual, delta)		PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL, "PCU_ASSERT_DOUBLE_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 1)
#define PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(expected, actual, delta)	PCU_assert_double_impl((expected), (actual), (delta), PCU_TYPE_DBL | PCU_TYPE_NOT, "PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #delta ")", __FILE__, __LINE__, 1)
#endif

#define PCU_FAIL_IMPL()		PCU_assert_impl(0, (size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL, "PCU_FAIL", __FILE__, __LINE__, 0)
#define PCU_FAIL_IMPL_F()	PCU_assert_impl(0, (size_t)(PCU_msg_buf), 0, PCU_TYPE_FAIL, "PCU_FAIL_FATAL", __FILE__, __LINE__, 1)

#define PCU_FAIL0(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, PCU_MSG_BUF_SIZE, format)                                    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL1(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1)                                ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL2(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2)                            ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL3(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3)                        ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL4(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4)                    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL5(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5)                ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL6(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6)            ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL7(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL8(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_FAIL_IMPL(); } while (0)

#define PCU_FAIL0_FATAL(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, PCU_MSG_BUF_SIZE, format)                                    ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL1_FATAL(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1)                                ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL2_FATAL(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2)                            ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL3_FATAL(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3)                        ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL4_FATAL(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4)                    ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL5_FATAL(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5)                ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL6_FATAL(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6)            ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL7_FATAL(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL8_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_FAIL_IMPL_F(); return; } while (0)
#define PCU_FAIL9_FATAL(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_FAIL_IMPL_F(); return; } while (0)

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC) && __STDC_VERSION__ >= 199901L
#define PCU_FAIL(format, ...)		do { PCU_snprintf(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, __VA_ARGS__); PCU_FAIL_IMPL(); } while (0)
#define PCU_FAIL_FATAL(format, ...)	do { PCU_snprintf(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, __VA_ARGS__); PCU_FAIL_IMPL_F(); return; } while (0)
#endif

int PCU_assert_impl(int passed_flag, size_t expected, size_t actual, unsigned long type, const char *expr, const char *file, int line, int fatal_flag);
#ifndef PCU_NO_FLOATINGPOINT
int PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *expr, const char *file, int line, int fatal_flag);
#endif


#define PCU_MSG_IMPL()	PCU_msg_impl(PCU_msg_buf, __FILE__, __LINE__)

#define PCU_MSG0(format)                                     do { PCU_SNPRINTF0(PCU_msg_buf, PCU_MSG_BUF_SIZE, format)                                    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG1(format, a1)                                 do { PCU_SNPRINTF1(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1)                                ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG2(format, a1, a2)                             do { PCU_SNPRINTF2(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2)                            ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG3(format, a1, a2, a3)                         do { PCU_SNPRINTF3(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3)                        ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG4(format, a1, a2, a3, a4)                     do { PCU_SNPRINTF4(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4)                    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG5(format, a1, a2, a3, a4, a5)                 do { PCU_SNPRINTF5(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5)                ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG6(format, a1, a2, a3, a4, a5, a6)             do { PCU_SNPRINTF6(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6)            ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG7(format, a1, a2, a3, a4, a5, a6, a7)         do { PCU_SNPRINTF7(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7)        ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG8(format, a1, a2, a3, a4, a5, a6, a7, a8)     do { PCU_SNPRINTF8(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8)    ; PCU_MSG_IMPL(); } while (0)
#define PCU_MSG9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) do { PCU_SNPRINTF9(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, a1, a2, a3, a4, a5, a6, a7, a8, a9); PCU_MSG_IMPL(); } while (0)

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC) && __STDC_VERSION__ >= 199901L
#define PCU_MSG(format, ...)		do { PCU_snprintf(PCU_msg_buf, PCU_MSG_BUF_SIZE, format, __VA_ARGS__); PCU_MSG_IMPL(); } while (0)
#endif

void PCU_msg_impl(const char *msg, const char *file, int line);


#ifdef __cplusplus
}
#endif

#endif /* PCUNIT_H_INCLUDED */
