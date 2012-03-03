#include "../../PCUnit/PCUnit.h"

static int initialize(void)
{
	return -1;
}

static int cleanup(void)
{
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


static void test_assert_initialize_err(void)
{
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

