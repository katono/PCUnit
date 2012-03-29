#include "../../PCUnit/PCUnit.h"
#include <stdio.h>
#include <string.h>

static FILE *fp_expected;
static FILE *fp_actual;

static int initialize(void)
{
	fp_expected = fopen("assert_not_supported_expected.txt", "r");
	fp_actual   = fopen("assert_not_supported_actual.txt", "r");
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


void output_test(FILE *fp_exp, FILE *fp_act);

static void test_not_supported(void)
{
	output_test(fp_expected, fp_actual);
}


PCU_Suite *AssertNotSupportedTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_not_supported),
	};
	static PCU_Suite suite = {
		"AssertNotSupportedTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

