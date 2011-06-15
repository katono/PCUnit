#include "include/PCUnit.h"
#include "include/PCU_TestCase.h"
#include "include/PCU_Test.h"
#include "include/PCU_Libc.h"

#define LIST_BEGIN(list)	(list)->next
#define LIST_END(list)		(list)
#define LIST_EMPTY(list)	((list)->next == list)

#ifndef PCU_MSG_BUF_SIZE
#define PCU_MSG_BUF_SIZE	512
#endif
char PCU_msg_buf[PCU_MSG_BUF_SIZE];
static char input_buf[64];
static int enable_color;

static PCU_Result result;

static void reset(PCU_TestCase *cases)
{
	PCU_TestCase *p;
	PCU_MEMSET(&result, 0, sizeof(result));
	for (p = cases; p->name != 0; p++) {
		PCU_TestCaseResult case_result;
		PCU_TestCase_reset(p);
		PCU_TestCase_get_result(p, &case_result);
		result.num_cases++;
		result.case_result.num_tests += case_result.num_tests;
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
#ifdef _WIN32
#include <windows.h>
static CONSOLE_SCREEN_BUFFER_INFO csbi;
static int csbi_init_flag = 0;
#endif
static void set_color(int color)
{
#ifdef _WIN32
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
#ifdef _WIN32
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

static void print_failure(PCU_Test *test)
{
	PCU_TestFailure *pos;
	PCU_TestFailure *list = &test->assertion_list;
	int n = 1;
	if (PCU_Test_is_failed(test)) {
		set_color(COLOR_RED);
		PCU_PRINTF0(" !!!! Failure !!!!\n");
	}
	if (!LIST_EMPTY(&test->assertion_list)) {
		PCU_PRINTF1(" Test: %s\n", test->name);
	}
	for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {
		unsigned long type;
		const char *true_str = "TRUE";
		const char *false_str = "FALSE";
		type = PCU_GET_ASSERT_TYPE(pos->type);
		if (type == PCU_TYPE_SETUP) {
			PCU_PRINTF2("  %d) %s\n", n++, test->name);
		} else {
			PCU_PRINTF3("  %d) %s:%d\n", n++, pos->file, pos->line);
		}
		PCU_PRINTF1("   %s\n", pos->expr);
		switch (type) {
		case PCU_TYPE_BOOL:
			PCU_PRINTF1("    expected <%s>\n", pos->expected.num ? true_str : false_str);
			PCU_PRINTF1("    actual   <%s>\n", pos->actual.num ? true_str : false_str);
			break;
		case PCU_TYPE_NUM:
			PCU_PRINTF2("    expected <%ld(0x%lx)>\n", pos->expected.num, pos->expected.num);
			PCU_PRINTF2("    actual   <%ld(0x%lx)>\n", pos->actual.num, pos->actual.num);
			break;
		case PCU_TYPE_PTR:
			if (pos->expected.ptr) {
				PCU_PRINTF1("    expected <%p>\n", pos->expected.ptr);
			} else {
				PCU_PRINTF0("    expected <NULL>\n");
			}
			if (pos->actual.ptr) {
				PCU_PRINTF1("    actual   <%p>\n", pos->actual.ptr);
			} else {
				PCU_PRINTF0("    actual   <NULL>\n");
			}
			break;
		case PCU_TYPE_STR:
		case PCU_TYPE_NSTR:
			if (pos->expected.str) {
				PCU_PRINTF1("    expected <\"%s\">\n", pos->expected.str);
			} else {
				PCU_PRINTF0("    expected <NULL>\n");
			}
			if (pos->actual.str) {
				PCU_PRINTF1("    actual   <\"%s\">\n", pos->actual.str);
			} else {
				PCU_PRINTF0("    actual   <NULL>\n");
			}
			if (type == PCU_TYPE_NSTR) {
				PCU_PRINTF1("    length   <%d>\n", PCU_GET_NSTR_LEN(pos->type));
			}
			break;
#if !defined(PCU_NO_FPU) && !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_STDLIB)
		case PCU_TYPE_DBL:
			PCU_PRINTF1("    expected <%g>\n", pos->expected.dbl);
			PCU_PRINTF1("    actual   <%g>\n", pos->actual.dbl);
			PCU_PRINTF1("    delta    <%g>\n", pos->delta);
			break;
#endif
		case PCU_TYPE_MSG:
		case PCU_TYPE_FAIL:
			PCU_PRINTF1("    %s\n", pos->expected.str);
			break;
		case PCU_TYPE_SETUP:
			PCU_PRINTF1("    return   <%ld>\n", pos->actual.num);
			break;
		default:
			break;
		}
		if (PCU_Test_is_repeated_test(test)) {
			PCU_PRINTF1("    repeat   <%d>\n", pos->repeat);
		}
	}
	if (!LIST_EMPTY(&test->assertion_list)) {
		PCU_PRINTF0("\n");
	}
	if (PCU_Test_is_failed(test)) {
		reset_color();
	}
}

static void print_result_selected(PCU_TestCase *testcase, int idx)
{
	PCU_Test *test = &testcase->tests[idx];
	print_failure(test);
	if (!PCU_Test_is_failed(test)) {
		set_color(COLOR_GREEN);
		PCU_PRINTF1("OK (%s)\n", test->name);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void print_result(PCU_TestCase *testcase)
{
	PCU_Test *p;
	PCU_PRINTF1("TestCase: %s\n", testcase->name);
	if (testcase->initialize_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("!!!! INITIALIZE FAILED <%d> !!!!\n", testcase->initialize_error);
		reset_color();
		PCU_PRINTF0("\n");
		return;
	}
	PCU_PRINTF0("\n");
	for (p = testcase->tests; p->name != 0; p++) {
		print_failure(p);
	}
	if (testcase->result.num_tests_failed == 0) {
		set_color(COLOR_GREEN);
		PCU_PRINTF1("OK (%d Tests)\n", testcase->result.num_tests_ran);
		reset_color();
		PCU_PRINTF0("\n");
	} else {
		set_color(COLOR_RED);
		PCU_PRINTF2("%d Tests, %d Failures\n", 
				testcase->result.num_tests_ran, testcase->result.num_tests_failed);
		reset_color();
		PCU_PRINTF0("\n");
	}
	if (testcase->cleanup_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("!!!! CLEANUP FAILED <%d> !!!!\n", testcase->cleanup_error);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void add_result(const PCU_TestCaseResult *c_result)
{
	result.case_result.num_tests_ran         += c_result->num_tests_ran;
	result.case_result.num_tests_failed      += c_result->num_tests_failed;
	result.case_result.num_errors_initialize += c_result->num_errors_initialize;
	result.case_result.num_errors_cleanup    += c_result->num_errors_cleanup;
	result.case_result.test_result.num_asserts         += c_result->test_result.num_asserts;
	result.case_result.test_result.num_asserts_ran     += c_result->test_result.num_asserts_ran;
	result.case_result.test_result.num_asserts_failed  += c_result->test_result.num_asserts_failed;
	result.case_result.test_result.num_errors_setup    += c_result->test_result.num_errors_setup;
	result.case_result.test_result.num_errors_teardown += c_result->test_result.num_errors_teardown;
	result.num_cases_ran++;
	if (c_result->num_tests_failed > 0) {
		result.num_cases_failed++;
	}
}

static void run_all(PCU_TestCase *cases)
{
	PCU_TestCase *p;
	for (p = cases; p->name != 0; p++) {
		PCU_TestCaseResult case_result;
		PCU_TestCase_run(p);
		PCU_TestCase_get_result(p, &case_result);
		add_result(&case_result);
		PCU_PRINTF0("----\n");
		print_result(p);
	}
	reset(cases);
}

static void run_selected_case(PCU_TestCase *cases, int case_idx)
{
	PCU_TestCaseResult case_result;
	PCU_TestCase_run(&cases[case_idx]);
	PCU_TestCase_get_result(&cases[case_idx], &case_result);
	add_result(&case_result);
	print_result(&cases[case_idx]);
	reset(cases);
}

static void run_selected_test(PCU_TestCase *cases, int case_idx, int test_idx)
{
	PCU_TestCaseResult case_result;
	PCU_TestCase_run_selected(&cases[case_idx], test_idx);
	PCU_TestCase_get_result(&cases[case_idx], &case_result);
	add_result(&case_result);
	print_result_selected(&cases[case_idx], test_idx);
	reset(cases);
}

static void show_list_tests(const PCU_Test *tests)
{
	int i;
	const PCU_Test *p;
	PCU_PRINTF0("List of Tests\n");
	PCU_PRINTF0("  Number  Name\n");
	for (i = 1, p = tests; p->name != 0; i++, p++) {
		PCU_PRINTF2("  %-6d  %s\n", i, p->name);
	}
	PCU_PRINTF0("\n");
}

static void show_list_cases(const PCU_TestCase *cases)
{
	int i;
	const PCU_TestCase *p;
	PCU_PRINTF0("List of TestCases\n");
	PCU_PRINTF0("  Number  Name\n");
	for (i = 1, p = cases; p->name != 0; i++, p++) {
		PCU_PRINTF2("  %-6d  %s\n", i, p->name);
	}
	PCU_PRINTF0("\n");
}

static int find_test_name(const PCU_Test *tests, const char *input_str)
{
	int i;
	const PCU_Test *p;
	for (i = 0, p = tests; p->name != 0; i++, p++) {
		if (PCU_STRCMP(p->name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int get_ntests(const PCU_Test *tests)
{
	int i;
	const PCU_Test *p;
	for (i = 0, p = tests; p->name != 0; i++, p++) ;
	return i;
}

static int find_test_number(const PCU_Test *tests, const char *input_str)
{
	int n;
	n = PCU_ATOI(input_str);
	if (n <= 0 || get_ntests(tests) + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static void get_line(char *buf, int size)
{
	int c;
	int i = 0;
	if (size <= 0) return;
	while (i < size - 1 && (c = PCU_getchar()) != '\n' && c != '\r') {
		buf[i++] = (char) c;
	}
	buf[i] = '\0';
}

static void select_test(PCU_TestCase *cases, int case_idx)
{
	int idx;
	PCU_TestCase *testcase = &cases[case_idx];
	PCU_PRINTF0("Enter Test's Number or Name: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_test_number(testcase->tests, input_buf);
	if (idx == -1) {
		idx = find_test_name(testcase->tests, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nTest not found.\n");
			return;
		}
	}
	PCU_PRINTF0("\n");

	run_selected_test(cases, case_idx, idx);
}

static int find_case_name(const PCU_TestCase *cases, const char *input_str)
{
	int i;
	const PCU_TestCase *p;
	for (i = 0, p = cases; p->name != 0; i++, p++) {
		if (PCU_STRCMP(p->name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int get_ncases(const PCU_TestCase *cases)
{
	int i;
	const PCU_TestCase *p;
	for (i = 0, p = cases; p->name != 0; i++, p++) ;
	return i;
}

static int find_case_number(const PCU_TestCase *cases, const char *input_str)
{
	int n;
	n = PCU_ATOI(input_str);
	if (n <= 0 || get_ncases(cases) + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static int select_case(PCU_TestCase *cases)
{
	int idx;
	PCU_TestCase *selected_case;
	PCU_PRINTF0("Enter TestCase's Number or Name: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_case_number(cases, input_buf);
	if (idx == -1) {
		idx = find_case_name(cases, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nTestCase not found.\n");
			return 0;
		}
	}

	selected_case = &cases[idx];

	PCU_PRINTF0("\n");
	while (1) {
		PCU_PRINTF1("============== TestCase: %s ==============\n", selected_case->name);
		PCU_PRINTF0("(R)un TestCase, (S)elect Test, (L)ist of Tests, (M)ove up, (Q)uit\n");
		PCU_PRINTF0("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_selected_case(cases, idx);
			break;
		case 's':
		case 'S':
			show_list_tests(selected_case->tests);
			select_test(cases, idx);
			break;
		case 'l':
		case 'L':
			show_list_tests(selected_case->tests);
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

void PCU_run_interactive(PCU_TestCase *suite)
{
	reset(suite);
	while (1) {
		int quit;
		PCU_PRINTF0("************** PCUnit: Interactive Mode **************\n");
		PCU_PRINTF1("(R)un all, (S)elect TestCase, (L)ist of TestCases, %sable color, (Q)uit\n", 
				enable_color ? "(D)is" : "(E)n");
		PCU_PRINTF0("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_all(suite);
			break;
		case 's':
		case 'S':
			show_list_cases(suite);
			quit = select_case(suite);
			if (quit) return;
			break;
		case 'l':
		case 'L':
			show_list_cases(suite);
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


void PCU_run(PCU_TestCase *suite)
{
	reset(suite);
	run_all(suite);
}

