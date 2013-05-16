#include "../../PCUnit/PCUnit.h"
#include <stdio.h>
#include <string.h>

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
	if (strcmp(PCU_test_name(), "test_assert_setup_err") == 0) {
		return -1;
	}
	return 0;
}

static int teardown(void)
{
	if (strcmp(PCU_test_name(), "test_assert_teardown_err") == 0) {
		return -2;
	}
	return 0;
}


static void test_assert_setup_err(void)
{
	printf("NOT REACHED\n");
	PCU_ASSERT(0);
}

static void test_assert_teardown_err(void)
{
	PCU_ASSERT(1);
}


static void test_skip(void)
{
	printf("NOT REACHED\n");
	PCU_ASSERT(0);
}

static void test_repeated(void)
{
	PCU_ASSERT_EQUAL(-1, PCU_repeat_counter());
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_setup_err),
	PCU_TEST(test_assert_teardown_err),
	PCU_TEST_SKIPPED(test_skip),
	PCU_TEST_REPEATED(test_repeated, 5),
};

PCU_Suite *SetupTest_suite(void)
{
	static PCU_Suite suite = {
		"SetupTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

