#include "../../PCUnit/PCUnit.h"
#include <stdio.h>

static int initialize(void)
{
	return -1;
}

static int cleanup(void)
{
	printf("NOT REACHED\n");
	return 0;
}

static int setup(void)
{
	printf("NOT REACHED\n");
	return 0;
}

static int teardown(void)
{
	printf("NOT REACHED\n");
	return 0;
}


static void test_assert_initialize_err(void)
{
	printf("NOT REACHED\n");
	PCU_ASSERT(1);
}


static PCU_Test tests[] = {
	PCU_TEST(test_assert_initialize_err),
};

PCU_Suite *InitializeTest_suite(void)
{
	static PCU_Suite suite = {
		"InitializeTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

