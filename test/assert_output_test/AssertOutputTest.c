#include "PCUnit/PCUnit.h"
#include <stdio.h>
#include <string.h>

static FILE *fp_expected;
static FILE *fp_actual;

static int initialize(void)
{
	fp_expected = fopen("assert_output_expected.txt", "r");
	fp_actual   = fopen("assert_output_actual.txt", "r");
	return !(fp_expected && fp_actual);
}

static int cleanup(void)
{
	fclose(fp_expected);
	fclose(fp_actual);
	return 0;
}

static int setup(void)
{
	return 0;
}

static int teardown(void)
{
	return 0;
}

static void line_check(const char *expected, const char *actual, int line)
{
	static int ptr_test_state = 0;
	const char *ptr_test_file = " AssertPtrTest.c";
	if (!strncmp(ptr_test_file, expected, strlen(ptr_test_file))) {
		ptr_test_state = 1;
	}
	if (ptr_test_state == 2 && !strcmp(expected, "")) {
		ptr_test_state = 0;
	}

	if (ptr_test_state == 2) {
		PCU_ASSERT_NSTRING_EQUAL_MESSAGE(expected, actual, 2, PCU_format("line:%d", line));
	} else {
		PCU_ASSERT_STRING_EQUAL_MESSAGE(expected, actual, PCU_format("line:%d", line));
	}

	if (ptr_test_state == 1) {
		ptr_test_state = 2;
	}
}

static void cut_newline(char *str, int line)
{
	char *p;
	p = strpbrk(str, "\r\n");
	PCU_ASSERT_PTR_NOT_NULL_MESSAGE(p, PCU_format("line:%d", line));
	*p = '\0';
}

static void test_output(void)
{
	char buf_expected[1024];
	char buf_actual[1024];
	int line = 1;
	while (1) {
		char *ret_expected, *ret_actual;
		ret_expected = fgets(buf_expected, sizeof buf_expected, fp_expected);
		ret_actual   = fgets(buf_actual, sizeof buf_actual, fp_actual);
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


PCU_Suite *AssertOutputTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_output),
	};
	static PCU_Suite suite = {
		"AssertOutputTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

