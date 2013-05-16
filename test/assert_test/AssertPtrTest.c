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


static void test_assert_ptr_equal_OK(void)
{
	int a;
	PCU_ASSERT_PTR_EQUAL(&a, &a);
	PCU_ASSERT_PTR_EQUAL(NULL, NULL);
}

static void test_assert_ptr_equal_FAIL(void)
{
	int a, b;
	PCU_ASSERT_PTR_EQUAL(&a, &b);
	printf("NOT REACHED\n");
}

static void test_assert_ptr_equal_MSG(void)
{
	int a, b;
	PCU_ASSERT_PTR_EQUAL_MESSAGE(&a, &a, PCU_format("%p, %p", &a, &b));
	PCU_ASSERT_PTR_EQUAL_MESSAGE(&a, &b, PCU_format("%p, %p", &a, &b));
}



static void test_assert_ptr_not_equal_OK(void)
{
	int a, b;
	PCU_ASSERT_PTR_NOT_EQUAL(&a, &b);
	PCU_ASSERT_PTR_NOT_EQUAL(&a, NULL);
}

static void test_assert_ptr_not_equal_FAIL(void)
{
	int a;
	PCU_ASSERT_PTR_NOT_EQUAL(&a, &a);
	printf("NOT REACHED\n");
}

static void test_assert_ptr_not_equal_MSG(void)
{
	int a, b;
	PCU_ASSERT_PTR_NOT_EQUAL_MESSAGE(&a, &b, PCU_format("%p, %p", &a, &b));
	PCU_ASSERT_PTR_NOT_EQUAL_MESSAGE(&a, &a, PCU_format("%p, %p", &a, &b));
}



static void test_assert_ptr_null_OK(void)
{
	int *p = NULL;
	PCU_ASSERT_PTR_NULL(p);
	PCU_ASSERT_PTR_NULL(NULL);
}

static void test_assert_ptr_null_FAIL(void)
{
	int a;
	int *p = NULL;
	p = &a;
	PCU_ASSERT_PTR_NULL(p);
	printf("NOT REACHED\n");
}

static void test_assert_ptr_null_MSG(void)
{
	int a;
	int *p = NULL;
	PCU_ASSERT_PTR_NULL_MESSAGE(p, PCU_format("%p", p));
	p = &a;
	PCU_ASSERT_PTR_NULL_MESSAGE(p, PCU_format("%p", p));
}



static void test_assert_ptr_not_null_OK(void)
{
	int a;
	int *p = NULL;
	p = &a;
	PCU_ASSERT_PTR_NOT_NULL(p);
	PCU_ASSERT_PTR_NOT_NULL(&a);
}

static void test_assert_ptr_not_null_FAIL(void)
{
	int *p = NULL;
	PCU_ASSERT_PTR_NOT_NULL(p);
	printf("NOT REACHED\n");
}

static void test_assert_ptr_not_null_MSG(void)
{
	int a;
	int *p = &a;
	PCU_ASSERT_PTR_NOT_NULL_MESSAGE(p, PCU_format("%p", p));
	p = NULL;
	PCU_ASSERT_PTR_NOT_NULL_MESSAGE(p, PCU_format("%p", p));
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_ptr_equal_OK),
	PCU_TEST(test_assert_ptr_equal_FAIL),
	PCU_TEST(test_assert_ptr_equal_MSG),
	PCU_TEST(test_assert_ptr_not_equal_OK),
	PCU_TEST(test_assert_ptr_not_equal_FAIL),
	PCU_TEST(test_assert_ptr_not_equal_MSG),
	PCU_TEST(test_assert_ptr_null_OK),
	PCU_TEST(test_assert_ptr_null_FAIL),
	PCU_TEST(test_assert_ptr_null_MSG),
	PCU_TEST(test_assert_ptr_not_null_OK),
	PCU_TEST(test_assert_ptr_not_null_FAIL),
	PCU_TEST(test_assert_ptr_not_null_MSG),
};

PCU_Suite *AssertPtrTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertPtrTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

