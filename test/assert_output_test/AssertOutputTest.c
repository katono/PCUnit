#include "../../PCUnit/PCUnit.h"
#include <stdio.h>
#include <string.h>

static FILE *fp_expected;
static FILE *fp_actual;

static const char * const file_map[][2] = {
	{"assert_output_expected.txt"        , "assert_output_actual.txt"},
	{"assert_output_verbose_expected.txt", "assert_output_verbose_actual.txt"},
};

static int initialize(void)
{
	return 0;
}

static int cleanup(void)
{
	return 0;
}

static int setup(void)
{
	int idx = !strcmp(PCU_test_name(), "test_output") ? 0 : 1;
	fp_expected = fopen(file_map[idx][0], "r");
	fp_actual   = fopen(file_map[idx][1], "r");
	return !(fp_expected && fp_actual);
}

static int teardown(void)
{
	fclose(fp_expected);
	fclose(fp_actual);
	return 0;
}

static void line_check(const char *expected, const char *actual, int line)
{
	static int state = 0;
	const char *ptr_test_file = " AssertPtrTest.c";
	const char *str_test_file = " AssertString";
	const char *mem_test_file = " AssertMemoryTest.c";
	if (!strncmp(ptr_test_file, expected, strlen(ptr_test_file)) || 
			!strncmp(str_test_file, expected, strlen(str_test_file)) || 
			!strncmp(mem_test_file, expected, strlen(mem_test_file))) {
		state = 1;
	}

	if (state == 2 && !strcmp(expected, "")) {
		state = 0;
	}

	if (state == 2) {
		const char *p = strstr(expected, "0x");
		if (!p) {
			p = strstr(expected, "(");
		}
		if (p) {
			PCU_ASSERT_NSTRING_EQUAL_MESSAGE(expected, actual, p - expected, PCU_format("line:%d", line));
		} else {
			PCU_ASSERT_STRING_EQUAL_MESSAGE(expected, actual, PCU_format("line:%d", line));
		}
	} else if (strstr(expected, "Test.c:")) {
		const char *p = strchr(expected, ':');
		const char *q;
		p++;
		PCU_ASSERT_NSTRING_EQUAL_MESSAGE(expected, actual, p - expected, PCU_format("line:%d", line));
		p = strrchr(expected, ':');
		q = strrchr(actual, ':');
		PCU_ASSERT_STRING_EQUAL_MESSAGE(p, q, PCU_format("line:%d", line));
	} else {
		PCU_ASSERT_STRING_EQUAL_MESSAGE(expected, actual, PCU_format("line:%d", line));
	}

	if (state == 1) {
		state = 2;
	}
}

static void cut_newline(char *str, int line)
{
	char *p;
	p = strpbrk(str, "\r\n");
	PCU_ASSERT_PTR_NOT_NULL_MESSAGE(p, PCU_format("line:%d", line));
	*p = '\0';
}

void output_test(FILE *fp_exp, FILE *fp_act)
{
	int line = 1;
	while (1) {
		char buf_expected[512];
		char buf_actual[512];
		char *ret_expected, *ret_actual;
		ret_expected = fgets(buf_expected, sizeof buf_expected, fp_exp);
		ret_actual   = fgets(buf_actual, sizeof buf_actual, fp_act);
		PCU_ASSERT_FALSE_MESSAGE( ret_expected && !ret_actual, PCU_format("line:%d", line));
		PCU_ASSERT_FALSE_MESSAGE(!ret_expected &&  ret_actual, PCU_format("line:%d", line));
		if (!ret_expected && !ret_actual) {
			break;
		}

		cut_newline(buf_expected, line);
		cut_newline(buf_actual, line);
		line_check(buf_expected, buf_actual, line);
		line++;
	}
}

static void test_output(void)
{
	output_test(fp_expected, fp_actual);
}

static void test_output_verbose(void)
{
	output_test(fp_expected, fp_actual);
}


PCU_Suite *AssertOutputTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_output),
		PCU_TEST(test_output_verbose),
	};
	static PCU_Suite suite = {
		"AssertOutputTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

