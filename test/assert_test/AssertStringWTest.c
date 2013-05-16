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


static void test_assert_strw_equal_OK(void)
{
	const wchar_t *a, *b;
	a = b = L"hoge";
	PCU_ASSERT_STRINGW_EQUAL(L"hoge", a);
	PCU_ASSERT_STRINGW_EQUAL(a, b);
}

static void test_assert_strw_equal_FAIL(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"piyo";
	PCU_ASSERT_STRINGW_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_strw_equal_MSG(void)
{
	const wchar_t *a, *b;
	a = b = L"hoge";
	PCU_ASSERT_STRINGW_EQUAL_MESSAGE(a, b, PCU_formatW(L"%ls, %ls", a, b));
	b = L"piyo";
	PCU_ASSERT_STRINGW_EQUAL_MESSAGE(a, b, PCU_formatW(L"%ls, %ls", a, b));
}

static void test_assert_strw_equal_FAIL_expect_null(void)
{
	const wchar_t *a;
	a = L"hoge";
	PCU_ASSERT_STRINGW_EQUAL(NULL, a);
}

static void test_assert_strw_equal_FAIL_actual_null(void)
{
	const wchar_t *a;
	a = L"hoge";
	PCU_ASSERT_STRINGW_EQUAL(a, NULL);
}

static void test_assert_strw_equal_OK_null_null(void)
{
	PCU_ASSERT_STRINGW_EQUAL(NULL, NULL);
}



static void test_assert_strw_not_equal_OK(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"piyo";
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);
}

static void test_assert_strw_not_equal_FAIL(void)
{
	const wchar_t *a, *b;
	a = b = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);
	printf("NOT REACHED\n");
}

static void test_assert_strw_not_equal_MSG(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"piyo";
	PCU_ASSERT_STRINGW_NOT_EQUAL_MESSAGE(a, b, PCU_formatW(L"%ls, %ls", a, b));
	b = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL_MESSAGE(a, b, PCU_formatW(L"%ls, %ls", a, b));
}

static void test_assert_strw_not_equal_OK_expect_null(void)
{
	const wchar_t *a;
	a = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL(NULL, a);
}

static void test_assert_strw_not_equal_OK_actual_null(void)
{
	const wchar_t *a;
	a = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, NULL);
}

static void test_assert_strw_not_equal_FAIL_null_null(void)
{
	PCU_ASSERT_STRINGW_NOT_EQUAL(NULL, NULL);
}





static void test_assert_nstrw_equal_OK(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 3);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 0);
}

static void test_assert_nstrw_equal_FAIL(void)
{
	const wchar_t *a, *b;
	a = L"hogu";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 4);
	printf("NOT REACHED\n");
}

static void test_assert_nstrw_equal_MSG(void)
{
	const wchar_t *a, *b;
	a = L"hogu";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_EQUAL_MESSAGE(a, b, 3, PCU_formatW(L"%ls, %ls", a, b));
	PCU_ASSERT_NSTRINGW_EQUAL_MESSAGE(a, b, 4, PCU_formatW(L"%ls, %ls", a, b));
}

static void test_assert_nstrw_equal_FAIL_expect_null(void)
{
	const wchar_t *a;
	a = L"hogu";
	PCU_ASSERT_NSTRINGW_EQUAL(NULL, a, 1);
}

static void test_assert_nstrw_equal_FAIL_actual_null(void)
{
	const wchar_t *a;
	a = L"hogu";
	PCU_ASSERT_NSTRINGW_EQUAL(a, NULL, 1);
}

static void test_assert_nstrw_equal_OK_null_null(void)
{
	PCU_ASSERT_NSTRINGW_EQUAL(NULL, NULL, 1);
}




static void test_assert_nstrw_not_equal_OK(void)
{
	const wchar_t *a, *b;
	a = L"hogu";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 5);
}

static void test_assert_nstrw_not_equal_FAIL(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 4);
	printf("NOT REACHED\n");
}

static void test_assert_nstrw_not_equal_MSG(void)
{
	const wchar_t *a, *b;
	a = L"hogu";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_MESSAGE(a, b, 4, PCU_formatW(L"%ls, %ls", a, b));
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_MESSAGE(a, b, 3, PCU_formatW(L"%ls, %ls", a, b));
}

static void test_assert_nstrw_not_equal_OK_expect_null(void)
{
	const wchar_t *a;
	a = L"hogu";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(NULL, a, 1);
}

static void test_assert_nstrw_not_equal_OK_actual_null(void)
{
	const wchar_t *a;
	a = L"hogu";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, NULL, 1);
}

static void test_assert_nstrw_not_equal_FAIL_null_null(void)
{
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(NULL, NULL, 1);
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_strw_equal_OK),
	PCU_TEST(test_assert_strw_equal_FAIL),
	PCU_TEST(test_assert_strw_equal_MSG),
	PCU_TEST(test_assert_strw_equal_FAIL_expect_null),
	PCU_TEST(test_assert_strw_equal_FAIL_actual_null),
	PCU_TEST(test_assert_strw_equal_OK_null_null),
	PCU_TEST(test_assert_strw_not_equal_OK),
	PCU_TEST(test_assert_strw_not_equal_FAIL),
	PCU_TEST(test_assert_strw_not_equal_MSG),
	PCU_TEST(test_assert_strw_not_equal_OK_expect_null),
	PCU_TEST(test_assert_strw_not_equal_OK_actual_null),
	PCU_TEST(test_assert_strw_not_equal_FAIL_null_null),
	PCU_TEST(test_assert_nstrw_equal_OK),
	PCU_TEST(test_assert_nstrw_equal_FAIL),
	PCU_TEST(test_assert_nstrw_equal_MSG),
	PCU_TEST(test_assert_nstrw_equal_FAIL_expect_null),
	PCU_TEST(test_assert_nstrw_equal_FAIL_actual_null),
	PCU_TEST(test_assert_nstrw_equal_OK_null_null),
	PCU_TEST(test_assert_nstrw_not_equal_OK),
	PCU_TEST(test_assert_nstrw_not_equal_FAIL),
	PCU_TEST(test_assert_nstrw_not_equal_MSG),
	PCU_TEST(test_assert_nstrw_not_equal_OK_expect_null),
	PCU_TEST(test_assert_nstrw_not_equal_OK_actual_null),
	PCU_TEST(test_assert_nstrw_not_equal_FAIL_null_null),
};

PCU_Suite *AssertStringWTest_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringWTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

