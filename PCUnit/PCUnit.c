#include "PCUnit.h"
#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

#define LIST_BEGIN(list)	(list)->next
#define LIST_END(list)		(list)

char PCU_msg_buf[PCU_MESSAGE_BUF_SIZE];
static int enable_color;

static PCU_Result result;
static PCU_SuiteResult suite_result;

static void reset(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	PCU_MEMSET(&result, 0, sizeof(result));
	for (i = 0; i < num; i++, method++) {
		PCU_Suite *p = (*method)();
		PCU_Suite_reset(p);
		PCU_Suite_get_result(p, &suite_result);
		result.num_suites++;
		result.suite_result.num_tests += suite_result.num_tests;
	}
}

void PCU_enable_color(void)
{
	enable_color = 1;
}

void PCU_disable_color(void)
{
	enable_color = 0;
}

enum {
	COLOR_GREEN,
	COLOR_RED
};
#if defined(_WIN32) && !defined(_WIN32_WCE)
#include <windows.h>
static CONSOLE_SCREEN_BUFFER_INFO csbi;
static int csbi_init_flag = 0;
#endif
static void set_color(int color)
{
#if defined(_WIN32) && !defined(_WIN32_WCE)
	HANDLE h;
	WORD attr;
	if (!enable_color) return;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!csbi_init_flag) {
		GetConsoleScreenBufferInfo(h, &csbi);
		csbi_init_flag = 1;
	}
	switch (color) {
	case COLOR_GREEN:
		attr = FOREGROUND_GREEN;
		break;
	case COLOR_RED:
		attr = FOREGROUND_RED;
		break;
	default:
		attr = csbi.wAttributes;
		break;
	}
	SetConsoleTextAttribute(h, attr);
#else
	if (!enable_color) return;
	switch (color) {
	case COLOR_GREEN:
		PCU_PRINTF0("\033[32m");
		break;
	case COLOR_RED:
		PCU_PRINTF0("\033[31m");
		break;
	default:
		break;
	}
#endif
}
static void reset_color(void)
{
#if defined(_WIN32) && !defined(_WIN32_WCE)
	HANDLE h;
	if (!enable_color) return;
	if (!csbi_init_flag) return;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, csbi.wAttributes);
#else
	if (!enable_color) return;
	PCU_PRINTF0("\033[m");
#endif
}

unsigned long PCU_get_num_type(size_t sizeof_expected, size_t sizeof_actual, int is_operator)
{
	if (sizeof_expected <= sizeof(char) && sizeof_actual <= sizeof(char)) {
		return is_operator ? PCU_TYPE_OP_CHAR : PCU_TYPE_NUM_CHAR;
	} else if (sizeof_expected <= sizeof(short) && sizeof_actual <= sizeof(short)) {
		return is_operator ? PCU_TYPE_OP_SHORT : PCU_TYPE_NUM_SHORT;
	} else if (sizeof_expected <= sizeof(int) && sizeof_actual <= sizeof(int)) {
		return is_operator ? PCU_TYPE_OP_INT : PCU_TYPE_NUM_INT;
	} else if (sizeof_expected <= sizeof(long) && sizeof_actual <= sizeof(long)) {
		return is_operator ? PCU_TYPE_OP_LONG : PCU_TYPE_NUM_LONG;
	} else if (sizeof_expected <= sizeof(size_t) && sizeof_actual <= sizeof(size_t)) {
		return is_operator ? PCU_TYPE_OP_SIZET : PCU_TYPE_NUM_SIZET;
	} else {
		return is_operator ? PCU_TYPE_OP_LLONG : PCU_TYPE_NUM_LLONG;
	}
}

#define IS_ASCII(c)		(' ' == 0x20 && 0x20 <= (c) && (c) <= 0x7e)

#define PRINT_EXPECTED_ACTUAL_AUX(type, str, size_t_num, type_num, len_str)	\
	do {\
		PCU_PRINTF3("        " str " : 0x%0*" len_str "x (%d)", sizeof(type) * 2, type_num, size_t_num);\
		if (IS_ASCII(size_t_num)) {\
			PCU_PRINTF1(" '%c'\n", size_t_num);\
		} else {\
			PCU_PRINTF0("\n");\
		}\
	} while (0)

#define PRINT_EXPECTED_ACTUAL_16BIT(type, expected_str, actual_str, len_str)	\
	do {\
		const type e = (type) pos->expected.num;\
		const type a = (type) pos->actual.num;\
		if (sizeof(int) == 2 && (e > 0xFFFF || a > 0xFFFF)) {\
			PCU_PRINTF2("        " expected_str " : 0x%0*" len_str "x\n", sizeof(type) * 2, e);\
			PCU_PRINTF2("        " actual_str   " : 0x%0*" len_str "x\n", sizeof(type) * 2, a);\
		} else {\
			PRINT_EXPECTED_ACTUAL_AUX(type, expected_str, pos->expected.num, e, len_str);\
			PRINT_EXPECTED_ACTUAL_AUX(type, actual_str  , pos->actual.num  , a, len_str);\
		}\
	} while (0)

#define PRINT_EXPECTED_ACTUAL(type, expected_str, actual_str, len_str)	\
	do {\
		const type e = (type) pos->expected.num;\
		const type a = (type) pos->actual.num;\
		PRINT_EXPECTED_ACTUAL_AUX(type, expected_str, pos->expected.num, e, len_str);\
		PRINT_EXPECTED_ACTUAL_AUX(type, actual_str  , pos->actual.num  , a, len_str);\
	} while (0)

static void print_failure(PCU_Test *test)
{
	PCU_TestFailure *pos;
	PCU_TestFailure *list = &test->failure_list;
	int n = 1;
	if (PCU_Test_is_failed(test)) {
		PCU_PRINTF1("  Test: %s ... FAILED\n", test->name);
	}
	for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {
		unsigned long type;
		type = PCU_GET_ASSERT_TYPE(pos->type);
		if (type == PCU_TYPE_SETUP) {
			PCU_PRINTF2("    %d. %s\n", n, test->name);
		} else {
			PCU_PRINTF3("    %d. %s:%u\n", n, pos->file, pos->line);
		}
		n++;
		PCU_PRINTF1("      %s\n", pos->expr);
		switch (type) {
		case PCU_TYPE_NUM_CHAR:
			PRINT_EXPECTED_ACTUAL(unsigned char, "expected", "actual  ", "");
			break;
		case PCU_TYPE_NUM_SHORT:
			PRINT_EXPECTED_ACTUAL(unsigned short, "expected", "actual  ", "");
			break;
		case PCU_TYPE_NUM_INT:
			PRINT_EXPECTED_ACTUAL(unsigned int, "expected", "actual  ", "");
			break;
		case PCU_TYPE_NUM_LONG:
#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
			PRINT_EXPECTED_ACTUAL_16BIT(unsigned long, "expected", "actual  ", "l");
#else
			PRINT_EXPECTED_ACTUAL(unsigned long, "expected", "actual  ", "l");
#endif
			break;
		case PCU_TYPE_NUM_SIZET:
#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
			PRINT_EXPECTED_ACTUAL_16BIT(size_t, "expected", "actual  ", "");
#else
			PRINT_EXPECTED_ACTUAL(size_t, "expected", "actual  ", "");
#endif
			break;
		case PCU_TYPE_OP_CHAR:
			PRINT_EXPECTED_ACTUAL(unsigned char, "lhs     ", "rhs     ", "");
			break;
		case PCU_TYPE_OP_SHORT:
			PRINT_EXPECTED_ACTUAL(unsigned short, "lhs     ", "rhs     ", "");
			break;
		case PCU_TYPE_OP_INT:
			PRINT_EXPECTED_ACTUAL(unsigned int, "lhs     ", "rhs     ", "");
			break;
		case PCU_TYPE_OP_LONG:
#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
			PRINT_EXPECTED_ACTUAL_16BIT(unsigned long, "lhs     ", "rhs     ", "l");
#else
			PRINT_EXPECTED_ACTUAL(unsigned long, "lhs     ", "rhs     ", "l");
#endif
			break;
		case PCU_TYPE_OP_SIZET:
#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
			PRINT_EXPECTED_ACTUAL_16BIT(size_t, "lhs     ", "rhs     ", "");
#else
			PRINT_EXPECTED_ACTUAL(size_t, "lhs     ", "rhs     ", "");
#endif
			break;
		case PCU_TYPE_PTR:
			if (pos->expected.ptr) {
				PCU_PRINTF1("        expected : %p\n", pos->expected.ptr);
			} else {
				PCU_PRINTF0("        expected : NULL\n");
			}
			if (pos->actual.ptr) {
				PCU_PRINTF1("        actual   : %p\n", pos->actual.ptr);
			} else {
				PCU_PRINTF0("        actual   : NULL\n");
			}
			break;
		case PCU_TYPE_STR:
		case PCU_TYPE_NSTR:
			if (pos->expected.str) {
				PCU_PRINTF1("        expected : \"%s\"\n", pos->expected.str);
			} else {
				PCU_PRINTF0("        expected : NULL\n");
			}
			if (pos->actual.str) {
				PCU_PRINTF1("        actual   : \"%s\"\n", pos->actual.str);
			} else {
				PCU_PRINTF0("        actual   : NULL\n");
			}
			if (type == PCU_TYPE_NSTR) {
				PCU_PRINTF1("        length   : %d\n", PCU_GET_NSTR_LEN(pos->type));
			}
			break;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		case PCU_TYPE_DBL:
			PCU_PRINTF1("        expected : %g\n", pos->expected.dbl);
			PCU_PRINTF1("        actual   : %g\n", pos->actual.dbl);
			PCU_PRINTF1("        delta    : %g\n", pos->delta);
			break;
#endif
		case PCU_TYPE_MSG:
		case PCU_TYPE_FAIL:
			PCU_PRINTF1("        %s\n", pos->expected.str);
			break;
		case PCU_TYPE_SETUP:
			PCU_PRINTF1("        return   : 0x%x\n", pos->actual.num);
			break;
		default:
			break;
		}
		if (PCU_Test_is_repeated_test(test)) {
			PCU_PRINTF1("        repeat   : %d\n", pos->repeat);
		}
	}
}

static void print_result(PCU_Suite *suite)
{
	int i;
	PCU_Test *p;
	PCU_PRINTF1("Suite: %s\n", suite->name);
	if (suite->initialize_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("INITIALIZE FAILED : 0x%x\n", suite->initialize_error);
		reset_color();
		PCU_PRINTF0("\n");
		return;
	}
	for (i = 0, p = suite->tests; i < suite->ntests; i++, p++) {
		print_failure(p);
	}
	PCU_PRINTF0("\n");
	if (suite->result.num_tests_failed == 0) {
		set_color(COLOR_GREEN);
		PCU_PRINTF2("%d Tests, %d Skipped\n", 
				suite->result.num_tests,
				suite->result.num_tests - suite->result.num_tests_ran);
		PCU_PRINTF0("OK\n");
		reset_color();
		PCU_PRINTF0("\n");
	} else {
		set_color(COLOR_RED);
		PCU_PRINTF3("%d Tests, %d Failures, %d Skipped\n", 
				suite->result.num_tests, suite->result.num_tests_failed,
				suite->result.num_tests - suite->result.num_tests_ran);
		reset_color();
		PCU_PRINTF0("\n");
	}
	if (suite->cleanup_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("CLEANUP FAILED : 0x%x\n", suite->cleanup_error);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void add_result(void)
{
	result.suite_result.num_tests_ran         += suite_result.num_tests_ran;
	result.suite_result.num_tests_failed      += suite_result.num_tests_failed;
	result.suite_result.num_errors_initialize += suite_result.num_errors_initialize;
	result.suite_result.num_errors_cleanup    += suite_result.num_errors_cleanup;
	result.suite_result.test_result.num_asserts         += suite_result.test_result.num_asserts;
	result.suite_result.test_result.num_asserts_ran     += suite_result.test_result.num_asserts_ran;
	result.suite_result.test_result.num_asserts_failed  += suite_result.test_result.num_asserts_failed;
	result.suite_result.test_result.num_errors_setup    += suite_result.test_result.num_errors_setup;
	result.suite_result.test_result.num_errors_teardown += suite_result.test_result.num_errors_teardown;
	result.num_suites_ran++;
	if (suite_result.num_tests_failed > 0) {
		result.num_suites_failed++;
	}
}

static void run_all(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	for (i = 0; i < num; i++, method++) {
		PCU_Suite *p = (*method)();
		PCU_Suite_run(p);
		PCU_Suite_get_result(p, &suite_result);
		add_result();
		PCU_PRINTF0("----\n");
		print_result(p);
	}
	reset(suite_methods, num);
}

void PCU_run(const PCU_SuiteMethod *suite_methods, int num)
{
	reset(suite_methods, num);
	run_all(suite_methods, num);
}

#ifndef PCU_NO_CONSOLE_RUN
static char input_buf[64];

static void print_result_selected(PCU_Suite *suite, int idx)
{
	PCU_Test *test = &suite->tests[idx];
	print_failure(test);
	PCU_PRINTF0("\n");
	if (PCU_Test_is_failed(test)) {
		set_color(COLOR_RED);
		PCU_PRINTF1("FAILED (%s)\n", test->name);
		reset_color();
		PCU_PRINTF0("\n");
	} else {
		set_color(COLOR_GREEN);
		PCU_PRINTF2("%s (%s)\n", PCU_Test_is_skipped(test) ? "Skipped" : "OK", test->name);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void run_selected_suite(const PCU_SuiteMethod *suite_methods, int num, int suite_idx)
{
	PCU_Suite *p = (suite_methods[suite_idx])();
	PCU_Suite_run(p);
	PCU_Suite_get_result(p, &suite_result);
	add_result();
	print_result(p);
	reset(suite_methods, num);
}

static void run_selected_test(const PCU_SuiteMethod *suite_methods, int num, int suite_idx, int test_idx)
{
	PCU_Suite *p = (suite_methods[suite_idx])();
	PCU_Suite_run_selected(p, test_idx);
	PCU_Suite_get_result(p, &suite_result);
	add_result();
	print_result_selected(p, test_idx);
	reset(suite_methods, num);
}

static void show_list_tests(const PCU_Test *tests, int num)
{
	int i;
	int j;
	const PCU_Test *p;
	PCU_PRINTF0("List of Tests\n");
	PCU_PRINTF0("  Number  Name\n");
	for (i = 0, j = 1, p = tests; i < num; i++, j++, p++) {
		PCU_PRINTF2("  %-6d  %s\n", j, p->name);
	}
	PCU_PRINTF0("\n");
}

static void show_list_suites(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	int j;
	const PCU_SuiteMethod *method = suite_methods;
	PCU_PRINTF0("List of Suites\n");
	PCU_PRINTF0("  Number  Name\n");
	for (i = 0, j = 1; i < num; i++, j++, method++) {
		const PCU_Suite *p = (*method)();
		PCU_PRINTF2("  %-6d  %s\n", j, p->name);
	}
	PCU_PRINTF0("\n");
}

static int find_test_name(const PCU_Test *tests, int num, const char *input_str)
{
	int i;
	const PCU_Test *p;
	for (i = 0, p = tests; i < num; i++, p++) {
		if (PCU_STRCMP(p->name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int find_number(int num, const char *input_str)
{
	int n;
	n = PCU_ATOI(input_str);
	if (n <= 0 || num + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static void get_line(char *buf, int size)
{
	int c;
	int i = 0;
	if (size <= 0) return;
	while (i < size - 1 && (c = PCU_getchar()) != '\n') {
		if (c != '\r') {
			buf[i++] = (char) c;
		}
	}
	buf[i] = '\0';
}

static void select_test(const PCU_SuiteMethod *suite_methods, int num, int suite_idx)
{
	int idx;
	PCU_Suite *suite = (suite_methods[suite_idx])();
	PCU_PRINTF0("Enter Number or Name of Test: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_number(suite->ntests, input_buf);
	if (idx == -1) {
		idx = find_test_name(suite->tests, suite->ntests, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nTest not found.\n");
			return;
		}
	}
	PCU_PRINTF0("\n");

	run_selected_test(suite_methods, num, suite_idx, idx);
}

static int find_suite_name(const PCU_SuiteMethod *suite_methods, int num, const char *input_str)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	for (i = 0; i < num; i++, method++) {
		const PCU_Suite *p = (*method)();
		if (PCU_STRCMP(p->name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int select_suite(const PCU_SuiteMethod *suite_methods, int num)
{
	int idx;
	PCU_Suite *selected_suite;
	PCU_PRINTF0("Enter Number or Name of Suite: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_number(num, input_buf);
	if (idx == -1) {
		idx = find_suite_name(suite_methods, num, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nSuite not found.\n");
			return 0;
		}
	}

	selected_suite = (suite_methods[idx])();

	PCU_PRINTF0("\n");
	while (1) {
		PCU_PRINTF1("============== Suite: %s ==============\n", selected_suite->name);
		PCU_PRINTF0("(R)un Suite, (S)elect Test, (L)ist of Tests, (M)ove up, (Q)uit\n");
		PCU_PRINTF0("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_selected_suite(suite_methods, num, idx);
			break;
		case 's':
		case 'S':
			show_list_tests(selected_suite->tests, selected_suite->ntests);
			select_test(suite_methods, num, idx);
			break;
		case 'l':
		case 'L':
			show_list_tests(selected_suite->tests, selected_suite->ntests);
			break;
		case 'm':
		case 'M':
			return 0;
		case 'q':
		case 'Q':
			return 1;
		default:
			break;
		}
		PCU_PRINTF0("\n");
	}
}

void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num)
{
	reset(suite_methods, num);
	while (1) {
		int quit;
		PCU_PRINTF0("************** PCUnit Console **************\n");
		PCU_PRINTF1("(R)un all, (S)elect Suite, (L)ist of Suites, %sable color, (Q)uit\n", 
				enable_color ? "(D)is" : "(E)n");
		PCU_PRINTF0("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_all(suite_methods, num);
			break;
		case 's':
		case 'S':
			show_list_suites(suite_methods, num);
			quit = select_suite(suite_methods, num);
			if (quit) return;
			break;
		case 'l':
		case 'L':
			show_list_suites(suite_methods, num);
			break;
		case 'e':
		case 'E':
			PCU_enable_color();
			break;
		case 'd':
		case 'D':
			PCU_disable_color();
			break;
		case 'q':
		case 'Q':
			return;
		default:
			break;
		}
		PCU_PRINTF0("\n");
	}
}
#endif
