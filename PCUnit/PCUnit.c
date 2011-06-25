#include "PCUnit.h"
#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

#define LIST_BEGIN(list)	(list)->next
#define LIST_END(list)		(list)
#define LIST_EMPTY(list)	((list)->next == list)

char PCU_msg_buf[PCU_MSG_BUF_SIZE];
static char input_buf[64];
static int enable_color;
static PCU_Mode pcu_mode = PCU_MODE_NORMAL;

static PCU_Result result;

static void reset(PCU_Suite *suites)
{
	PCU_Suite *p;
	PCU_MEMSET(&result, 0, sizeof(result));
	for (p = suites; p->name != 0; p++) {
		PCU_SuiteResult suite_result;
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
#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
			if (sizeof(int) < sizeof(size_t) && 
					(pos->expected.num > 0xFFFF || pos->actual.num > 0xFFFF)) {
				PCU_PRINTF1("    expected <0x%x>\n", pos->expected.num);
				PCU_PRINTF1("    actual   <0x%x>\n", pos->actual.num);
			} else {
				PCU_PRINTF2("    expected <%d(0x%x)>\n", pos->expected.num, pos->expected.num);
				PCU_PRINTF2("    actual   <%d(0x%x)>\n", pos->actual.num, pos->actual.num);
			}
#else
			PCU_PRINTF2("    expected <%d(0x%x)>\n", pos->expected.num, pos->expected.num);
			PCU_PRINTF2("    actual   <%d(0x%x)>\n", pos->actual.num, pos->actual.num);
#endif
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
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
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
			PCU_PRINTF1("    return   <0x%x>\n", pos->actual.num);
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

static void print_result_selected(PCU_Suite *suite, int idx)
{
	PCU_Test *test = &suite->tests[idx];
	print_failure(test);
	if (!PCU_Test_is_failed(test)) {
		set_color(COLOR_GREEN);
		PCU_PRINTF1("OK (%s)\n", test->name);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void print_result(PCU_Suite *suite)
{
	PCU_Test *p;
	PCU_PRINTF1("Suite: %s\n", suite->name);
	if (suite->initialize_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("!!!! INITIALIZE FAILED <0x%x> !!!!\n", suite->initialize_error);
		reset_color();
		PCU_PRINTF0("\n");
		return;
	}
	PCU_PRINTF0("\n");
	for (p = suite->tests; p->name != 0; p++) {
		print_failure(p);
	}
	if (suite->result.num_tests_failed == 0) {
		set_color(COLOR_GREEN);
		PCU_PRINTF1("OK (%d Tests)\n", suite->result.num_tests_ran);
		reset_color();
		PCU_PRINTF0("\n");
	} else {
		set_color(COLOR_RED);
		PCU_PRINTF2("%d Tests, %d Failures\n", 
				suite->result.num_tests_ran, suite->result.num_tests_failed);
		reset_color();
		PCU_PRINTF0("\n");
	}
	if (suite->cleanup_error) {
		set_color(COLOR_RED);
		PCU_PRINTF1("!!!! CLEANUP FAILED <0x%x> !!!!\n", suite->cleanup_error);
		reset_color();
		PCU_PRINTF0("\n");
	}
}

static void add_result(const PCU_SuiteResult *suite_result)
{
	result.suite_result.num_tests_ran         += suite_result->num_tests_ran;
	result.suite_result.num_tests_failed      += suite_result->num_tests_failed;
	result.suite_result.num_errors_initialize += suite_result->num_errors_initialize;
	result.suite_result.num_errors_cleanup    += suite_result->num_errors_cleanup;
	result.suite_result.test_result.num_asserts         += suite_result->test_result.num_asserts;
	result.suite_result.test_result.num_asserts_ran     += suite_result->test_result.num_asserts_ran;
	result.suite_result.test_result.num_asserts_failed  += suite_result->test_result.num_asserts_failed;
	result.suite_result.test_result.num_errors_setup    += suite_result->test_result.num_errors_setup;
	result.suite_result.test_result.num_errors_teardown += suite_result->test_result.num_errors_teardown;
	result.num_suites_ran++;
	if (suite_result->num_tests_failed > 0) {
		result.num_suites_failed++;
	}
}

static void run_all(PCU_Suite *suites)
{
	PCU_Suite *p;
	for (p = suites; p->name != 0; p++) {
		PCU_SuiteResult suite_result;
		PCU_Suite_run(p);
		PCU_Suite_get_result(p, &suite_result);
		add_result(&suite_result);
		PCU_PRINTF0("----\n");
		print_result(p);
	}
	reset(suites);
}

static void run_selected_suite(PCU_Suite *suites, int suite_idx)
{
	PCU_SuiteResult suite_result;
	PCU_Suite_run(&suites[suite_idx]);
	PCU_Suite_get_result(&suites[suite_idx], &suite_result);
	add_result(&suite_result);
	print_result(&suites[suite_idx]);
	reset(suites);
}

static void run_selected_test(PCU_Suite *suites, int suite_idx, int test_idx)
{
	PCU_SuiteResult suite_result;
	PCU_Suite_run_selected(&suites[suite_idx], test_idx);
	PCU_Suite_get_result(&suites[suite_idx], &suite_result);
	add_result(&suite_result);
	print_result_selected(&suites[suite_idx], test_idx);
	reset(suites);
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

static void show_list_suites(const PCU_Suite *suites)
{
	int i;
	const PCU_Suite *p;
	PCU_PRINTF0("List of Suites\n");
	PCU_PRINTF0("  Number  Name\n");
	for (i = 1, p = suites; p->name != 0; i++, p++) {
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

static void select_test(PCU_Suite *suites, int suite_idx)
{
	int idx;
	PCU_Suite *suite = &suites[suite_idx];
	PCU_PRINTF0("Enter Test's Number or Name: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_test_number(suite->tests, input_buf);
	if (idx == -1) {
		idx = find_test_name(suite->tests, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nTest not found.\n");
			return;
		}
	}
	PCU_PRINTF0("\n");

	run_selected_test(suites, suite_idx, idx);
}

static int find_suite_name(const PCU_Suite *suites, const char *input_str)
{
	int i;
	const PCU_Suite *p;
	for (i = 0, p = suites; p->name != 0; i++, p++) {
		if (PCU_STRCMP(p->name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int get_nsuites(const PCU_Suite *suites)
{
	int i;
	const PCU_Suite *p;
	for (i = 0, p = suites; p->name != 0; i++, p++) ;
	return i;
}

static int find_suite_number(const PCU_Suite *suites, const char *input_str)
{
	int n;
	n = PCU_ATOI(input_str);
	if (n <= 0 || get_nsuites(suites) + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static int select_suite(PCU_Suite *suites)
{
	int idx;
	PCU_Suite *selected_suite;
	PCU_PRINTF0("Enter Suite's Number or Name: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_suite_number(suites, input_buf);
	if (idx == -1) {
		idx = find_suite_name(suites, input_buf);
		if (idx == -1) {
			PCU_PRINTF0("\nSuite not found.\n");
			return 0;
		}
	}

	selected_suite = &suites[idx];

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
			run_selected_suite(suites, idx);
			break;
		case 's':
		case 'S':
			show_list_tests(selected_suite->tests);
			select_test(suites, idx);
			break;
		case 'l':
		case 'L':
			show_list_tests(selected_suite->tests);
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

static void run_interactive(PCU_Suite *suites)
{
	while (1) {
		int quit;
		PCU_PRINTF0("************** PCUnit: Interactive Mode **************\n");
		PCU_PRINTF1("(R)un all, (S)elect Suite, (L)ist of Suites, %sable color, (Q)uit\n", 
				enable_color ? "(D)is" : "(E)n");
		PCU_PRINTF0("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_all(suites);
			break;
		case 's':
		case 'S':
			show_list_suites(suites);
			quit = select_suite(suites);
			if (quit) return;
			break;
		case 'l':
		case 'L':
			show_list_suites(suites);
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

void PCU_run(PCU_Suite *suites)
{
	reset(suites);
	switch (pcu_mode) {
	case PCU_MODE_INTERACTIVE:
		run_interactive(suites);
		break;
	case PCU_MODE_NORMAL:
	default:
		run_all(suites);
		break;
	}
}

void PCU_set_mode(PCU_Mode mode)
{
	pcu_mode = mode;
}

