#include "PCUnit.h"
#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

static int enable_color;

typedef struct {
	int num_tests;
	int num_tests_ran;
	int num_tests_failed;
} PCU_Result;
static PCU_Result result;

static void reset(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	PCU_MEMSET(&result, 0, sizeof(result));
	for (i = 0; i < num; i++, method++) {
		PCU_Suite *p = (*method)();
		PCU_Suite_reset(p);
		result.num_tests += p->result.num_tests;
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
		PCU_puts("\033[32m");
		break;
	case COLOR_RED:
		PCU_puts("\033[31m");
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
	PCU_puts("\033[m");
#endif
}

static void print_before_test(const PCU_Suite *suite)
{
	PCU_puts("\n");
	PCU_puts("Suite: ");
	PCU_puts(suite->name);
	PCU_puts("\n");
}

static void print_initialize_error(const PCU_Suite *suite)
{
	set_color(COLOR_RED);
	PCU_PRINTF1("INITIALIZE FAILED : %d\n", suite->result.initialize_error);
	reset_color();
	PCU_puts("\n");
}

static void print_cleanup_error(const PCU_Suite *suite)
{
	set_color(COLOR_RED);
	PCU_PRINTF1("CLEANUP FAILED : %d\n", suite->result.cleanup_error);
	reset_color();
	PCU_puts("\n");
}

static void print_after_test(const PCU_Suite *suite)
{
	if (suite->result.initialize_error) {
		print_initialize_error(suite);
		return;
	}
	if (suite->result.num_tests_failed == 0) {
		set_color(COLOR_GREEN);
	} else {
		set_color(COLOR_RED);
	}
	PCU_PRINTF3("%d Tests, %d Failures, %d Skipped\n", 
			suite->result.num_tests, suite->result.num_tests_failed,
			suite->result.num_tests - suite->result.num_tests_ran);
	if (suite->result.num_tests_failed == 0) {
		PCU_puts("OK\n");
	}
	reset_color();
	PCU_puts("\n");
	if (suite->result.cleanup_error) {
		print_cleanup_error(suite);
	}
}

static void add_result(const PCU_Suite *suite)
{
	result.num_tests_ran    += suite->result.num_tests_ran;
	result.num_tests_failed += suite->result.num_tests_failed;
	if (suite->result.initialize_error) {
		result.num_tests_failed++;
	}
	if (suite->result.cleanup_error) {
		result.num_tests_failed++;
	}
}

static void run_all(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	reset(suite_methods, num);
	for (i = 0; i < num; i++, method++) {
		PCU_Suite *p = (*method)();
		print_before_test(p);
		PCU_Suite_run(p);
		add_result(p);
		PCU_puts("\n");
		print_after_test(p);
	}
}

int PCU_run(const PCU_SuiteMethod *suite_methods, int num)
{
	run_all(suite_methods, num);
	return result.num_tests_failed > 0;
}

#ifndef PCU_NO_CONSOLE_RUN
static char input_buf[64];

static void print_result_selected(const PCU_Suite *suite, int idx)
{
	PCU_Test *test = &suite->tests[idx];
	if (suite->result.initialize_error) {
		print_initialize_error(suite);
		return;
	}
	PCU_puts("\n");
	if (PCU_Test_is_failed(test)) {
		set_color(COLOR_RED);
		PCU_puts("FAILED (");
		PCU_puts(test->name);
		PCU_puts(")\n");
		reset_color();
		PCU_puts("\n");
	} else {
		set_color(COLOR_GREEN);
		if (PCU_Test_is_skipped(test)) {
			PCU_puts("Skipped (");
		} else {
			PCU_puts("OK (");
		}
		PCU_puts(test->name);
		PCU_puts(")\n");
		reset_color();
		PCU_puts("\n");
	}
	if (suite->result.cleanup_error) {
		print_cleanup_error(suite);
	}
}

static void run_selected_suite(const PCU_SuiteMethod *suite_methods, int num, int suite_idx)
{
	PCU_Suite *p = (suite_methods[suite_idx])();
	reset(suite_methods, num);
	print_before_test(p);
	PCU_Suite_run(p);
	add_result(p);
	PCU_puts("\n");
	print_after_test(p);
}

static void run_selected_test(const PCU_SuiteMethod *suite_methods, int num, int suite_idx, int test_idx)
{
	PCU_Suite *p = (suite_methods[suite_idx])();
	reset(suite_methods, num);
	PCU_Suite_run_selected(p, test_idx);
	add_result(p);
	print_result_selected(p, test_idx);
}

static void show_list_tests(const PCU_Test *tests, int num)
{
	int i;
	const PCU_Test *p;
	PCU_puts("List of Tests\n");
	PCU_puts("  Number  Name\n");
	for (i = 0, p = tests; i < num; i++, p++) {
		PCU_PRINTF1("  %-6d  ", i + 1);
		PCU_puts(p->name);
		PCU_puts("\n");
	}
	PCU_puts("\n");
}

static void show_list_suites(const PCU_SuiteMethod *suite_methods, int num)
{
	int i;
	const PCU_SuiteMethod *method = suite_methods;
	PCU_puts("List of Suites\n");
	PCU_puts("  Number  Name\n");
	for (i = 0; i < num; i++, method++) {
		const PCU_Suite *p = (*method)();
		PCU_PRINTF1("  %-6d  ", i + 1);
		PCU_puts(p->name);
		PCU_puts("\n");
	}
	PCU_puts("\n");
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
	PCU_puts("Enter Number or Name of Test: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_number(suite->ntests, input_buf);
	if (idx == -1) {
		idx = find_test_name(suite->tests, suite->ntests, input_buf);
		if (idx == -1) {
			PCU_puts("\nTest not found.\n");
			return;
		}
	}
	PCU_puts("\n");

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
	PCU_puts("Enter Number or Name of Suite: ");
	get_line(input_buf, sizeof input_buf);

	idx = find_number(num, input_buf);
	if (idx == -1) {
		idx = find_suite_name(suite_methods, num, input_buf);
		if (idx == -1) {
			PCU_puts("\nSuite not found.\n");
			return 0;
		}
	}

	selected_suite = (suite_methods[idx])();

	PCU_puts("\n");
	while (1) {
		PCU_puts("============== Suite: ");
		PCU_puts(selected_suite->name);
		PCU_puts(" ==============\n");
		PCU_puts("(R)un Suite, (S)elect Test, (L)ist of Tests, (M)ove up, (Q)uit\n");
		PCU_puts("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_puts("\n");
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
		PCU_puts("\n");
	}
	return 0;
}

void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num)
{
	reset(suite_methods, num);
	while (1) {
		int quit;
		PCU_puts("************** PCUnit Console **************\n");
		PCU_puts("(R)un all, (S)elect Suite, (L)ist of Suites, ");
		if (enable_color) {
			PCU_puts("(D)is");
		} else {
			PCU_puts("(E)n");
		}
		PCU_puts("able color, (Q)uit\n");
		PCU_puts("Enter Command: ");
		get_line(input_buf, sizeof input_buf);
		PCU_puts("\n");
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
		PCU_puts("\n");
	}
}
#endif
