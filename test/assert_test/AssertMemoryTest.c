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

typedef struct STRUCT_t {
	int x;
	char y;
} STRUCT_t;

static void test_assert_mem_equal_OK(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 4;
	PCU_ASSERT_MEMORY_EQUAL(&a[0], &b[0], sizeof a, 1);
	PCU_ASSERT_MEMORY_EQUAL(&a[0], &b[0], sizeof a[0], 2);
}

static void test_assert_mem_char_equal_OK(void)
{
	const char *a, *b;
	a = "hoge";
	b = "hogehoge";
	PCU_ASSERT_MEMORY_EQUAL("hoge", a, 4, 1);
	PCU_ASSERT_MEMORY_EQUAL(a, b, 4, 1);
	PCU_ASSERT_MEMORY_EQUAL(a, b, 3, 1);
	PCU_ASSERT_MEMORY_EQUAL(a, b, 0, 1);
}

static void test_assert_mem_equal_FAIL(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_EQUAL(&a[0], &b[0], sizeof a[0], 2);
	printf("NOT REACHED\n");
}

static void test_assert_mem_equal_MSG(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_EQUAL_MESSAGE(&a[0], &b[0], sizeof a[0], 1, PCU_format("%d, %d", a[0].x, b[0].y));
	PCU_ASSERT_MEMORY_EQUAL_MESSAGE(&a[0], &b[0], sizeof a[0], 2, PCU_format("%d, %d", a[1].x, b[1].y));
}

static void test_assert_mem_equal_FAIL_expect_null(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_EQUAL(NULL, &b[0], sizeof a[0], 1);
}

static void test_assert_mem_equal_FAIL_actual_null(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_EQUAL(&a[0], NULL, sizeof a[0], 1);
}

static void test_assert_mem_equal_OK_null_null(void)
{
	PCU_ASSERT_MEMORY_EQUAL(NULL, NULL, 4, 2);
}




static void test_assert_mem_not_equal_OK(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_NOT_EQUAL(&a[0], &b[0], sizeof a, 1);
	PCU_ASSERT_MEMORY_NOT_EQUAL(&a[0], &b[0], sizeof a[0], 2);
}

static void test_assert_mem_char_not_equal_OK(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_MEMORY_NOT_EQUAL("hoge", a, 4, 1);
	PCU_ASSERT_MEMORY_NOT_EQUAL(a, b, 4, 1);
	PCU_ASSERT_MEMORY_NOT_EQUAL(a, b, 5, 1);
}

static void test_assert_mem_not_equal_FAIL(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 4;
	PCU_ASSERT_MEMORY_NOT_EQUAL(&a[0], &b[0], sizeof a[0], 2);
	printf("NOT REACHED\n");
}

static void test_assert_mem_not_equal_MSG(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 4;
	PCU_ASSERT_MEMORY_NOT_EQUAL_MESSAGE(&a[0], &b[0], sizeof a[0], 1, PCU_format("%d, %d", a[0].x, b[0].y));
	PCU_ASSERT_MEMORY_NOT_EQUAL_MESSAGE(&a[0], &b[0], sizeof a[0], 2, PCU_format("%d, %d", a[1].x, b[1].y));
}

static void test_assert_mem_not_equal_OK_expect_null(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_NOT_EQUAL(NULL, &b[0], sizeof a[0], 1);
}

static void test_assert_mem_not_equal_OK_actual_null(void)
{
	STRUCT_t a[2] = {{0}};
	STRUCT_t b[2] = {{0}};

	a[0].x = 1; a[0].y = 2; a[1].x = 3; a[1].y = 4;
	b[0].x = 1; b[0].y = 2; b[1].x = 3; b[1].y = 5;
	PCU_ASSERT_MEMORY_NOT_EQUAL(&a[0], NULL, sizeof a[0], 1);
}

static void test_assert_mem_not_equal_FAIL_null_null(void)
{
	PCU_ASSERT_MEMORY_NOT_EQUAL(NULL, NULL, 1, 4);
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_mem_equal_OK),
	PCU_TEST(test_assert_mem_char_equal_OK),
	PCU_TEST(test_assert_mem_equal_FAIL),
	PCU_TEST(test_assert_mem_equal_MSG),
	PCU_TEST(test_assert_mem_equal_FAIL_expect_null),
	PCU_TEST(test_assert_mem_equal_FAIL_actual_null),
	PCU_TEST(test_assert_mem_equal_OK_null_null),
	PCU_TEST(test_assert_mem_not_equal_OK),
	PCU_TEST(test_assert_mem_char_not_equal_OK),
	PCU_TEST(test_assert_mem_not_equal_FAIL),
	PCU_TEST(test_assert_mem_not_equal_MSG),
	PCU_TEST(test_assert_mem_not_equal_OK_expect_null),
	PCU_TEST(test_assert_mem_not_equal_OK_actual_null),
	PCU_TEST(test_assert_mem_not_equal_FAIL_null_null),
};

PCU_Suite *AssertMemoryTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertMemoryTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

