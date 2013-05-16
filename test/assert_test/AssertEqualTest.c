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


static void test_assert_equal_OK(void)
{
	int a, b;
	a = 100;
	b = 100;
	PCU_ASSERT_EQUAL(a, b);
}

static void test_assert_equal_FAIL(void)
{
	int a, b;
	a = 100;
	b = 99;
	PCU_ASSERT_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_equal_MSG(void)
{
	int a, b;
	a = 100;
	b = 100;
	PCU_ASSERT_EQUAL_MESSAGE(a, b, PCU_format("%d, %d", a, b));
	b++;
	PCU_ASSERT_EQUAL_MESSAGE(a, b, PCU_format("%d, %d", a, b));
}

static void test_assert_equal_char_FAIL(void)
{
	char a, b;
	a = 100;
	b = -1;
	PCU_ASSERT_EQUAL(a, b);
}

static void test_assert_equal_short_FAIL(void)
{
	short a, b;
	a = 100;
	b = -1;
	PCU_ASSERT_EQUAL(a, b);
}

static void test_assert_equal_long_FAIL(void)
{
	long a, b;
	a = 100;
	b = -1;
	PCU_ASSERT_EQUAL(a, b);
}

static void test_assert_equal_longlong_FAIL(void)
{
	long long a, b;
	a = 100;
	b = -1;
	PCU_ASSERT_EQUAL(a, b);
}




static void test_assert_not_equal_OK(void)
{
	int a, b;
	a = 100;
	b = 99;
	PCU_ASSERT_NOT_EQUAL(a, b);
}

static void test_assert_not_equal_FAIL(void)
{
	int a, b;
	a = 100;
	b = 100;
	PCU_ASSERT_NOT_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_not_equal_MSG(void)
{
	int a, b;
	a = 100;
	b = 99;
	PCU_ASSERT_NOT_EQUAL_MESSAGE(a, b, PCU_format("%d, %d", a, b));
	b++;
	PCU_ASSERT_NOT_EQUAL_MESSAGE(a, b, PCU_format("%d, %d", a, b));
}




static PCU_Test tests[] = {
	PCU_TEST(test_assert_equal_OK),
	PCU_TEST(test_assert_equal_FAIL),
	PCU_TEST(test_assert_equal_MSG),
	PCU_TEST(test_assert_equal_char_FAIL),
	PCU_TEST(test_assert_equal_short_FAIL),
	PCU_TEST(test_assert_equal_long_FAIL),
	PCU_TEST(test_assert_equal_longlong_FAIL),
	PCU_TEST(test_assert_not_equal_OK),
	PCU_TEST(test_assert_not_equal_FAIL),
	PCU_TEST(test_assert_not_equal_MSG),
};

PCU_Suite *AssertEqualTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertEqualTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

