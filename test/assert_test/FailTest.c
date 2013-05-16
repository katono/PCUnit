#include "../../PCUnit/PCUnit.h"
#include <stdio.h>

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
	return 0;
}

static int teardown(void)
{
	return 0;
}


static void test_fail1(void)
{
	PCU_FAIL("test_fail");
	printf("NOT REACHED\n");
}

static void test_fail2(void)
{
	PCU_FAIL(PCU_format("test_fail: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10));
}

static void test_message1(void)
{
	PCU_MESSAGE("test_message");
}

static void test_message2(void)
{
	PCU_MESSAGE(PCU_format("test_message: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10));
}

static void test_message_fail(void)
{
	PCU_MESSAGE("test_message");
	PCU_FAIL("test_fail");
}


static PCU_Test tests[] = {
	PCU_TEST(test_fail1),
	PCU_TEST(test_fail2),
	PCU_TEST(test_message1),
	PCU_TEST(test_message2),
	PCU_TEST(test_message_fail),
};

PCU_Suite *FailTest_suite(void)
{
	static PCU_Suite suite = {
		"FailTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

