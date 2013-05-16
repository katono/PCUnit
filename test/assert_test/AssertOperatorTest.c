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



static void test_assert_operator_OK(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR(0, <, 1);
	PCU_ASSERT_OPERATOR(0, <=, 1);
	PCU_ASSERT_OPERATOR(-1, >, 1);
	PCU_ASSERT_OPERATOR(-1, >=, 1);
	PCU_ASSERT_OPERATOR(a, ==, b);
	PCU_ASSERT_OPERATOR(a, ^, b+1);
	PCU_ASSERT_OPERATOR(a, &, b);
	PCU_ASSERT_OPERATOR(a, |, b);
}

static void test_assert_operator_FAIL1(void)
{
	PCU_ASSERT_OPERATOR(-1, <, 1);
	printf("NOT REACHED\n");
}

static void test_assert_operator_FAIL2(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR(a, ^, b);
}

static void test_assert_operator_FAIL3(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR(a, &, ~b);
}

static void test_assert_operator_FAIL4(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x101;
	PCU_ASSERT_OPERATOR(a, ==, b);
}

static void test_assert_operator_FAIL5(void)
{
	unsigned int a;
	a = 0x100;
	PCU_ASSERT_OPERATOR(0x10 <= a, &&, a < 0x100);
}

static void test_assert_operator_FAIL6(void)
{
	unsigned int a;
	a = 0x100;
	PCU_ASSERT_OPERATOR(a < 0x10, ||, 0x100 < a);
}

static void test_assert_operator_MSG(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_MESSAGE(a, ==, b, PCU_format("%d, %d", a, b));
	b = 0x0;
	PCU_ASSERT_OPERATOR_MESSAGE(a, ==, b, PCU_format("%d, %d", a, b));
}



static void test_assert_operator_int_OK(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_INT(0, <, 1);
	PCU_ASSERT_OPERATOR_INT(0, <=, 1);
	PCU_ASSERT_OPERATOR_INT(-1, <, 1);
	PCU_ASSERT_OPERATOR_INT(-1, <=, 1);
	PCU_ASSERT_OPERATOR_INT(a, ==, b);
	PCU_ASSERT_OPERATOR_INT(a, ^, b+1);
	PCU_ASSERT_OPERATOR_INT(a, &, b);
	PCU_ASSERT_OPERATOR_INT(a, |, b);
}

static void test_assert_operator_int_FAIL1(void)
{
	PCU_ASSERT_OPERATOR_INT(-1, >, 1);
}

static void test_assert_operator_int_FAIL2(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_INT(a, ^, b);
}

static void test_assert_operator_int_FAIL3(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_INT(a, &, ~b);
}

static void test_assert_operator_int_FAIL4(void)
{
	int a, b;
	a = 0x100;
	b = 0x101;
	PCU_ASSERT_OPERATOR_INT(a, ==, b);
}

static void test_assert_operator_int_FAIL5(void)
{
	int a;
	a = 0x100;
	PCU_ASSERT_OPERATOR_INT(0x10 <= a, &&, a < 0x100);
}

static void test_assert_operator_int_FAIL6(void)
{
	int a;
	a = 0x100;
	PCU_ASSERT_OPERATOR_INT(a < 0x10, ||, 0x100 < a);
}

static void test_assert_operator_int_MSG(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_INT_MESSAGE(a, ==, b, PCU_format("%d, %d", a, b));
	b = 0x0;
	PCU_ASSERT_OPERATOR_INT_MESSAGE(a, ==, b, PCU_format("%d, %d", a, b));
}



static void test_assert_operator_dbl_OK(void)
{
	double a, b;
	a = 100.0;
	b = 100.0;
	PCU_ASSERT_OPERATOR_DOUBLE(0, <, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(0, <=, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, <, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, <=, 1);

	PCU_ASSERT_OPERATOR_DOUBLE(a, ==, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, <=, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, <, b + 0.01);
}

static void test_assert_operator_dbl_FAIL1(void)
{
	PCU_ASSERT_OPERATOR_DOUBLE(0, >, 1);
}

static void test_assert_operator_dbl_FAIL2(void)
{
	double a, b;
	a = 100.0;
	b = 100.0;
	PCU_ASSERT_OPERATOR_DOUBLE(a, <, b);
}

static void test_assert_operator_dbl_FAIL3(void)
{
	double a, b;
	a = 100.0;
	b = 100.1;
	PCU_ASSERT_OPERATOR_DOUBLE(a, ==, b);
}


static PCU_Test tests[] = {
	PCU_TEST(test_assert_operator_OK),
	PCU_TEST(test_assert_operator_FAIL1),
	PCU_TEST(test_assert_operator_FAIL2),
	PCU_TEST(test_assert_operator_FAIL3),
	PCU_TEST(test_assert_operator_FAIL4),
	PCU_TEST(test_assert_operator_FAIL5),
	PCU_TEST(test_assert_operator_FAIL6),
	PCU_TEST(test_assert_operator_MSG),
	PCU_TEST(test_assert_operator_int_OK),
	PCU_TEST(test_assert_operator_int_FAIL1),
	PCU_TEST(test_assert_operator_int_FAIL2),
	PCU_TEST(test_assert_operator_int_FAIL3),
	PCU_TEST(test_assert_operator_int_FAIL4),
	PCU_TEST(test_assert_operator_int_FAIL5),
	PCU_TEST(test_assert_operator_int_FAIL6),
	PCU_TEST(test_assert_operator_int_MSG),
	PCU_TEST(test_assert_operator_dbl_OK),
	PCU_TEST(test_assert_operator_dbl_FAIL1),
	PCU_TEST(test_assert_operator_dbl_FAIL2),
	PCU_TEST(test_assert_operator_dbl_FAIL3),
};

PCU_Suite *AssertOperatorTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertOperatorTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

