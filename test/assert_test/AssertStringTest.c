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


static void test_assert_str_equal_OK(void)
{
	const char *a, *b;
	a = b = "hoge";
	PCU_ASSERT_STRING_EQUAL("hoge", a);
	PCU_ASSERT_STRING_EQUAL(a, b);
}

static void test_assert_str_equal_FAIL(void)
{
	const char *a, *b;
	a = "hoge";
	b = "piyo";
	PCU_ASSERT_STRING_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_str_equal_MSG(void)
{
	const char *a, *b;
	a = b = "hoge";
	PCU_ASSERT_STRING_EQUAL_MESSAGE(a, b, PCU_format("%s, %s", a, b));
	b = "piyo";
	PCU_ASSERT_STRING_EQUAL_MESSAGE(a, b, PCU_format("%s, %s", a, b));
}

static void test_assert_str_equal_FAIL_expect_null(void)
{
	const char *a;
	a = "hoge";
	PCU_ASSERT_STRING_EQUAL(NULL, a);
}

static void test_assert_str_equal_FAIL_actual_null(void)
{
	const char *a;
	a = "hoge";
	PCU_ASSERT_STRING_EQUAL(a, NULL);
}

static void test_assert_str_equal_OK_null_null(void)
{
	PCU_ASSERT_STRING_EQUAL(NULL, NULL);
}



static void test_assert_str_not_equal_OK(void)
{
	const char *a, *b;
	a = "hoge";
	b = "piyo";
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);
}

static void test_assert_str_not_equal_FAIL(void)
{
	const char *a, *b;
	a = b = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_str_not_equal_MSG(void)
{
	const char *a, *b;
	a = "hoge";
	b = "piyo";
	PCU_ASSERT_STRING_NOT_EQUAL_MESSAGE(a, b, PCU_format("%s, %s", a, b));
	b = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL_MESSAGE(a, b, PCU_format("%s, %s", a, b));
}

static void test_assert_str_not_equal_OK_expect_null(void)
{
	const char *a;
	a = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL(NULL, a);
}

static void test_assert_str_not_equal_OK_actual_null(void)
{
	const char *a;
	a = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL(a, NULL);
}

static void test_assert_str_not_equal_FAIL_null_null(void)
{
	PCU_ASSERT_STRING_NOT_EQUAL(NULL, NULL);
}





static void test_assert_nstr_equal_OK(void)
{
	const char *a, *b;
	a = "hoge";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 3);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 0);
}

static void test_assert_nstr_equal_FAIL(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_EQUAL(a, b, 4);
	printf("NOT REACHED\n");
}

static void test_assert_nstr_equal_MSG(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_EQUAL_MESSAGE(a, b, 3, PCU_format("%s, %s", a, b));
	PCU_ASSERT_NSTRING_EQUAL_MESSAGE(a, b, 4, PCU_format("%s, %s", a, b));
}

static void test_assert_nstr_equal_FAIL_expect_null(void)
{
	const char *a;
	a = "hogu";
	PCU_ASSERT_NSTRING_EQUAL(NULL, a, 1);
}

static void test_assert_nstr_equal_FAIL_actual_null(void)
{
	const char *a;
	a = "hogu";
	PCU_ASSERT_NSTRING_EQUAL(a, NULL, 1);
}

static void test_assert_nstr_equal_OK_null_null(void)
{
	PCU_ASSERT_NSTRING_EQUAL(NULL, NULL, 1);
}




static void test_assert_nstr_not_equal_OK(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_NOT_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 5);
}

static void test_assert_nstr_not_equal_FAIL(void)
{
	const char *a, *b;
	a = "hoge";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 4);
	printf("NOT REACHED\n");
}

static void test_assert_nstr_not_equal_MSG(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_NOT_EQUAL_MESSAGE(a, b, 4, PCU_format("%s, %s", a, b));
	PCU_ASSERT_NSTRING_NOT_EQUAL_MESSAGE(a, b, 3, PCU_format("%s, %s", a, b));
}

static void test_assert_nstr_not_equal_OK_expect_null(void)
{
	const char *a;
	a = "hogu";
	PCU_ASSERT_NSTRING_NOT_EQUAL(NULL, a, 1);
}

static void test_assert_nstr_not_equal_OK_actual_null(void)
{
	const char *a;
	a = "hogu";
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, NULL, 1);
}

static void test_assert_nstr_not_equal_FAIL_null_null(void)
{
	PCU_ASSERT_NSTRING_NOT_EQUAL(NULL, NULL, 1);
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_str_equal_OK),
	PCU_TEST(test_assert_str_equal_FAIL),
	PCU_TEST(test_assert_str_equal_MSG),
	PCU_TEST(test_assert_str_equal_FAIL_expect_null),
	PCU_TEST(test_assert_str_equal_FAIL_actual_null),
	PCU_TEST(test_assert_str_equal_OK_null_null),
	PCU_TEST(test_assert_str_not_equal_OK),
	PCU_TEST(test_assert_str_not_equal_FAIL),
	PCU_TEST(test_assert_str_not_equal_MSG),
	PCU_TEST(test_assert_str_not_equal_OK_expect_null),
	PCU_TEST(test_assert_str_not_equal_OK_actual_null),
	PCU_TEST(test_assert_str_not_equal_FAIL_null_null),
	PCU_TEST(test_assert_nstr_equal_OK),
	PCU_TEST(test_assert_nstr_equal_FAIL),
	PCU_TEST(test_assert_nstr_equal_MSG),
	PCU_TEST(test_assert_nstr_equal_FAIL_expect_null),
	PCU_TEST(test_assert_nstr_equal_FAIL_actual_null),
	PCU_TEST(test_assert_nstr_equal_OK_null_null),
	PCU_TEST(test_assert_nstr_not_equal_OK),
	PCU_TEST(test_assert_nstr_not_equal_FAIL),
	PCU_TEST(test_assert_nstr_not_equal_MSG),
	PCU_TEST(test_assert_nstr_not_equal_OK_expect_null),
	PCU_TEST(test_assert_nstr_not_equal_OK_actual_null),
	PCU_TEST(test_assert_nstr_not_equal_FAIL_null_null),
};

PCU_Suite *AssertStringTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

