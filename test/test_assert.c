#include <stdio.h>
#include "../PCUnit/PCUnit.h"


static void test_assert(void)
{
	PCU_ASSERT(1);
	PCU_ASSERT(0);
	PCU_ASSERT_FATAL(1);
	PCU_ASSERT_FATAL(0);
	PCU_ASSERT(0);
}

static void test_assert_true(void)
{
	int a = 1;
	PCU_ASSERT_TRUE(a);
	PCU_ASSERT_TRUE_FATAL(a);
	a = 0;
	PCU_ASSERT_TRUE(a);
	PCU_ASSERT_TRUE_FATAL(a);
	PCU_ASSERT_TRUE(a);
}

static void test_assert_false(void)
{
	int a = 0;
	PCU_ASSERT_FALSE(a);
	PCU_ASSERT_FALSE_FATAL(a);
	a = 1;
	PCU_ASSERT_FALSE(a);
	PCU_ASSERT_FALSE_FATAL(a);
	PCU_ASSERT_FALSE(a);
}

static void test_assert_equal(void)
{
	int a, b;
	a = 100;
	b = 100;
	PCU_ASSERT_EQUAL(-1, 1);
	PCU_ASSERT_EQUAL((long long)-1, 1);
	PCU_ASSERT_EQUAL((unsigned long long)-1, 1);
	PCU_ASSERT_EQUAL((size_t)-1, 1);
	PCU_ASSERT_EQUAL((unsigned short)-1, 1);
	PCU_ASSERT_EQUAL((unsigned char)-1, 1);
	PCU_ASSERT_EQUAL(a, b);
	PCU_ASSERT_EQUAL_FATAL(a, b);
	b++;
	PCU_ASSERT_EQUAL(a, b);
	PCU_ASSERT_EQUAL_FATAL(a, b);
	PCU_ASSERT_EQUAL(a, b);
}

static void test_assert_not_equal(void)
{
	int a, b;
	a = 100;
	b = 99;
	PCU_ASSERT_NOT_EQUAL(a, b);
	PCU_ASSERT_NOT_EQUAL_FATAL(a, b);
	b++;
	PCU_ASSERT_NOT_EQUAL(a, b);
	PCU_ASSERT_NOT_EQUAL_FATAL(a, b);
	PCU_ASSERT_NOT_EQUAL(a, b);
}

static void test_assert_ptr_equal(void)
{
	int a, b;
	PCU_ASSERT_PTR_EQUAL(&a, &a);
	PCU_ASSERT_PTR_EQUAL(NULL, NULL);
	PCU_ASSERT_PTR_EQUAL_FATAL(&a, &a);

	PCU_ASSERT_PTR_EQUAL(&a, &b);
	PCU_ASSERT_PTR_EQUAL(NULL, &a);
	PCU_ASSERT_PTR_EQUAL_FATAL(&a, &b);
	PCU_ASSERT_PTR_EQUAL(&a, &b);
}

static void test_assert_ptr_not_equal(void)
{
	int a, b;
	PCU_ASSERT_PTR_NOT_EQUAL(&a, &b);
	PCU_ASSERT_PTR_NOT_EQUAL(NULL, &a);
	PCU_ASSERT_PTR_NOT_EQUAL_FATAL(&a, &b);

	PCU_ASSERT_PTR_NOT_EQUAL(&a, &a);
	PCU_ASSERT_PTR_NOT_EQUAL(NULL, NULL);
	PCU_ASSERT_PTR_NOT_EQUAL_FATAL(&a, &a);
	PCU_ASSERT_PTR_NOT_EQUAL(NULL, NULL);

}

static void test_assert_ptr_null(void)
{
	int a;
	int *p = NULL;
	PCU_ASSERT_PTR_NULL(p);
	PCU_ASSERT_PTR_NULL(NULL);
	PCU_ASSERT_PTR_NULL_FATAL(p);
	PCU_ASSERT_PTR_NULL_FATAL(NULL);

	p = &a;
	PCU_ASSERT_PTR_NULL(p);
	PCU_ASSERT_PTR_NULL(&a);
	PCU_ASSERT_PTR_NULL_FATAL(p);
	PCU_ASSERT_PTR_NULL(p);
}

static void test_assert_ptr_not_null(void)
{
	int a;
	int *p = &a;
	PCU_ASSERT_PTR_NOT_NULL(p);
	PCU_ASSERT_PTR_NOT_NULL(&a);
	PCU_ASSERT_PTR_NOT_NULL_FATAL(p);

	p = NULL;
	PCU_ASSERT_PTR_NOT_NULL(p);
	PCU_ASSERT_PTR_NOT_NULL(NULL);
	PCU_ASSERT_PTR_NOT_NULL_FATAL(p);
	PCU_ASSERT_PTR_NOT_NULL(NULL);

}

static void test_assert_str_equal(void)
{
	const char *a, *b;
	a = b = "hoge";
	PCU_ASSERT_STRING_EQUAL("hoge", a);
	PCU_ASSERT_STRING_EQUAL(a, b);
	PCU_ASSERT_STRING_EQUAL_FATAL(a, b);

	a = "piyo";
	PCU_ASSERT_STRING_EQUAL("hoge", a);
	PCU_ASSERT_STRING_EQUAL(a, b);
	PCU_ASSERT_STRING_EQUAL(NULL, b);
	PCU_ASSERT_STRING_EQUAL(a, NULL);
	PCU_ASSERT_STRING_EQUAL(NULL, NULL);
	PCU_ASSERT_STRING_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRING_EQUAL(a, b);
}

static void test_assert_str_not_equal(void)
{
	const char *a, *b;
	a = "piyo";
	b = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL("hoge", a);
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);
	PCU_ASSERT_STRING_NOT_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);

	a = b = "hoge";
	PCU_ASSERT_STRING_NOT_EQUAL("hoge", a);
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);
	PCU_ASSERT_STRING_NOT_EQUAL(NULL, b);
	PCU_ASSERT_STRING_NOT_EQUAL(a, NULL);
	PCU_ASSERT_STRING_NOT_EQUAL(NULL, NULL);
	PCU_ASSERT_STRING_NOT_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRING_NOT_EQUAL(a, b);
}

static void test_assert_nstr_equal(void)
{
	const char *a, *b;
	a = "hoge";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_EQUAL_FATAL(a, b, 3);
	PCU_ASSERT_NSTRING_EQUAL_FATAL(a, b, 0);

	a = "hogu";
	PCU_ASSERT_NSTRING_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 5);
	PCU_ASSERT_NSTRING_EQUAL(NULL, b, 1);
	PCU_ASSERT_NSTRING_EQUAL(a, NULL, 2);
	PCU_ASSERT_NSTRING_EQUAL(NULL, NULL, 0);
	PCU_ASSERT_NSTRING_EQUAL_FATAL(a, b, 4);
	PCU_ASSERT_NSTRING_EQUAL(a, b, 4);
}

static void test_assert_nstr_not_equal(void)
{
	const char *a, *b;
	a = "hogu";
	b = "hogehoge";
	PCU_ASSERT_NSTRING_NOT_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 5);
	PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(a, b, 4);

	a = "hoge";
	PCU_ASSERT_NSTRING_NOT_EQUAL("hoge", a, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRING_NOT_EQUAL(NULL, b, 1);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, NULL, 2);
	PCU_ASSERT_NSTRING_NOT_EQUAL(NULL, NULL, 0);
	PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(a, b, 3);
	PCU_ASSERT_NSTRING_NOT_EQUAL(a, b, 4);
}

static void test_assert_strw_equal(void)
{
	const wchar_t *a, *b;
	a = b = L"hoge";
	PCU_ASSERT_STRINGW_EQUAL(L"hoge", a);
	PCU_ASSERT_STRINGW_EQUAL(a, b);
	PCU_ASSERT_STRINGW_EQUAL_FATAL(a, b);

	a = L"piyo";
	PCU_ASSERT_STRINGW_EQUAL(L"hoge", a);
	PCU_ASSERT_STRINGW_EQUAL(a, b);
	PCU_ASSERT_STRINGW_EQUAL(NULL, b);
	PCU_ASSERT_STRINGW_EQUAL(a, NULL);
	PCU_ASSERT_STRINGW_EQUAL(NULL, NULL);
	PCU_ASSERT_STRINGW_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRINGW_EQUAL(a, b);
}

static void test_assert_strw_not_equal(void)
{
	const wchar_t *a, *b;
	a = L"piyo";
	b = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL(L"hoge", a);
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);
	PCU_ASSERT_STRINGW_NOT_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);

	a = b = L"hoge";
	PCU_ASSERT_STRINGW_NOT_EQUAL(L"hoge", a);
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);
	PCU_ASSERT_STRINGW_NOT_EQUAL(NULL, b);
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, NULL);
	PCU_ASSERT_STRINGW_NOT_EQUAL(NULL, NULL);
	PCU_ASSERT_STRINGW_NOT_EQUAL_FATAL(a, b);
	PCU_ASSERT_STRINGW_NOT_EQUAL(a, b);
}

static void test_assert_nstrw_equal(void)
{
	const wchar_t *a, *b;
	a = L"hoge";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_EQUAL_FATAL(a, b, 3);
	PCU_ASSERT_NSTRINGW_EQUAL_FATAL(a, b, 0);

	a = L"hogu";
	PCU_ASSERT_NSTRINGW_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 5);
	PCU_ASSERT_NSTRINGW_EQUAL(NULL, b, 1);
	PCU_ASSERT_NSTRINGW_EQUAL(a, NULL, 2);
	PCU_ASSERT_NSTRINGW_EQUAL(NULL, NULL, 0);
	PCU_ASSERT_NSTRINGW_EQUAL_FATAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_EQUAL(a, b, 4);
}

static void test_assert_nstrw_not_equal(void)
{
	const wchar_t *a, *b;
	a = L"hogu";
	b = L"hogehoge";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 5);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_FATAL(a, b, 4);

	a = L"hoge";
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(L"hoge", a, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 4);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(NULL, b, 1);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, NULL, 2);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(NULL, NULL, 0);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_FATAL(a, b, 3);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(a, b, 4);
}

static void test_assert_dbl_equal(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.0;
	delta = 0.0;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL_FATAL(a, b, delta);

	a = 100.0;
	b = 100.1;
	delta = 0.1;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL_FATAL(a, b, delta);

	a = 100.1;
	b = 100.0;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL_FATAL(a, b, delta);

	delta = -0.1;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL_FATAL(a, b, delta);

	delta = 0.0999;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL_FATAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
}
static void test_assert_dbl_not_equal(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.0999;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(a, b, delta);

	delta = -0.0999;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(a, b, delta);

	delta = 0.1;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);

	a = 100.0;
	b = 100.0;
	delta = 0.0;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(a, b, delta);
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
}

static void test_msg0(void)
{
	PCU_MESSAGE0("test_msg0: 1");
	PCU_MESSAGE0("test_msg0: 2");
	PCU_FAIL0("test_msg0: 3");
	PCU_FAIL0_FATAL("test_msg0: 4");
	PCU_FAIL0("test_msg0: 4");
}

static void test_msg1(void)
{
	PCU_MESSAGE1("test_msg1: %d", 1);
	PCU_MESSAGE1("test_msg1: %d", 2);
	PCU_FAIL1("test_msg1: %d", 3);
	PCU_FAIL1_FATAL("test_msg1: %d", 4);
	PCU_FAIL1("test_msg1: %d", 5);
}

static void test_msg2(void)
{
	PCU_MESSAGE2("test_msg2: %d,%#x", 1, 2);
	PCU_MESSAGE2("test_msg2: %d,%#x", 2, 2);
	PCU_FAIL2("test_msg2: %d,%#x", 3, 2);
	PCU_FAIL2_FATAL("test_msg2: %d,%#x", 4, 2);
	PCU_FAIL2("test_msg2: %d,%#x", 5, 2);
}

static void test_msg3(void)
{
	PCU_MESSAGE3("test_msg3: %d,%#x, %c", 1, 2, '3');
	PCU_MESSAGE3("test_msg3: %d,%#x, %c", 2, 2, '3');
	PCU_FAIL3("test_msg3: %d,%#x, %c", 3, 2, '3');
	PCU_FAIL3_FATAL("test_msg3: %d,%#x, %c", 4, 2, '3');
	PCU_FAIL3("test_msg3: %d,%#x, %c", 5, 2, '3');
}

static void test_msg4(void)
{
	PCU_MESSAGE4("test_msg4: %d,%#x, %c, %04d", 1, 2, '3', 4);
	PCU_MESSAGE4("test_msg4: %d,%#x, %c, %04d", 2, 2, '3', 4);
	PCU_FAIL4("test_msg4: %d,%#x, %c, %04d", 3, 2, '3', 4);
	PCU_FAIL4_FATAL("test_msg4: %d,%#x, %c, %04d", 4, 2, '3', 4);
	PCU_FAIL4("test_msg4: %d,%#x, %c, %04d", 5, 2, '3', 4);
}

static void test_msg5(void)
{
	PCU_MESSAGE5("test_msg5: %d,%#x, %c, %04d, %s", 1, 2, '3', 4, "5");
	PCU_MESSAGE5("test_msg5: %d,%#x, %c, %04d, %s", 2, 2, '3', 4, "5");
	PCU_FAIL5("test_msg5: %d,%#x, %c, %04d, %s", 3, 2, '3', 4, "5");
	PCU_FAIL5_FATAL("test_msg5: %d,%#x, %c, %04d, %s", 4, 2, '3', 4, "5");
	PCU_FAIL5("test_msg5: %d,%#x, %c, %04d, %s", 5, 2, '3', 4, "5");
}

static void test_msg6(void)
{
	PCU_MESSAGE6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 1, 2, '3', 4, "5", 6);
	PCU_MESSAGE6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 2, 2, '3', 4, "5", 6);
	PCU_FAIL6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 3, 2, '3', 4, "5", 6);
	PCU_FAIL6_FATAL("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 4, 2, '3', 4, "5", 6);
	PCU_FAIL6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 5, 2, '3', 4, "5", 6);
}

static void test_msg7(void)
{
	PCU_MESSAGE7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 1, 2, '3', 4, "5", 6, 7);
	PCU_MESSAGE7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 2, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 3, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7_FATAL("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 4, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 5, 2, '3', 4, "5", 6, 7);
}

static void test_msg8(void)
{
	PCU_MESSAGE8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 1, 2, '3', 4, "5", 6, 7, 8);
	PCU_MESSAGE8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 2, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 3, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8_FATAL("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 4, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 5, 2, '3', 4, "5", 6, 7, 8);
}

static void test_msg9(void)
{
	PCU_MESSAGE9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 1, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_MESSAGE9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 2, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 3, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9_FATAL("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 4, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 5, 2, '3', 4, "5", 6, 7, 8, -9);
}

static void test_msg(void)
{
	PCU_MESSAGE("hoge");
	PCU_FAIL("piyo");
	PCU_MESSAGE("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_MESSAGE("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 2, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 3, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL_FATAL("foo");
	PCU_FAIL_FATAL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 4, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 5, 2, '3', 4, "5", 6, 7, 8, -9, 10);
}

static void test_msgw(void)
{
	PCU_MESSAGEW(L"hoge");
	PCU_FAILW(L"piyo");
	PCU_MESSAGEW(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
	PCU_MESSAGEW(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 2, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
	PCU_FAILW(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 3, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
	PCU_FAILW_FATAL(L"foo");
	PCU_FAILW_FATAL(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 4, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
	PCU_FAILW(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 5, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
}

static void test_assert_dummy(void)
{
	PCU_ASSERT(0);
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


static void test_skip(void)
{
	PCU_ASSERT(0);
}


static void test_assert_operator(void)
{
	unsigned int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR(-1, ==, 0x1FFFFFFFFULL);
	PCU_ASSERT_OPERATOR(0, <, 1);
	PCU_ASSERT_OPERATOR(0, <=, 1);
	PCU_ASSERT_OPERATOR(0, >, 1);
	PCU_ASSERT_OPERATOR(0, >=, 1);
	PCU_ASSERT_OPERATOR(-1, <, 1);
	PCU_ASSERT_OPERATOR(-1, <=, 1);
	PCU_ASSERT_OPERATOR(-1, >, 1);
	PCU_ASSERT_OPERATOR(-1, >=, 1);
	PCU_ASSERT_OPERATOR(a, ==, b);
	PCU_ASSERT_OPERATOR(a, ^, b);
	PCU_ASSERT_OPERATOR(a, ^, b+1);
	PCU_ASSERT_OPERATOR(a, &, b);
	PCU_ASSERT_OPERATOR(a, &, ~b);
	PCU_ASSERT_OPERATOR(a, |, b);
	b++;
	PCU_ASSERT_OPERATOR(a, ==, b);
	b = 0;
	PCU_ASSERT_OPERATOR(a, &, b);
	PCU_ASSERT_OPERATOR(0, |, b);
	PCU_ASSERT_OPERATOR(0x10 <= a, &&, a < 0x100);
	PCU_ASSERT_OPERATOR(a < 0x10, ||, 0x100 < a);
	PCU_ASSERT_OPERATOR_FATAL(0, <, 1);
	PCU_ASSERT_OPERATOR_FATAL(0, ==, 1);
	PCU_ASSERT_OPERATOR(0, >, 1);
}

static void test_assert_operator_int(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR_INT(-1, ==, 0x1FFFFFFFFULL);
	PCU_ASSERT_OPERATOR_INT(0, <, 1);
	PCU_ASSERT_OPERATOR_INT(0, <=, 1);
	PCU_ASSERT_OPERATOR_INT(0, >, 1);
	PCU_ASSERT_OPERATOR_INT(0, >=, 1);
	PCU_ASSERT_OPERATOR_INT(-1, <, 1);
	PCU_ASSERT_OPERATOR_INT(-1, <=, 1);
	PCU_ASSERT_OPERATOR_INT(-1, >, 1);
	PCU_ASSERT_OPERATOR_INT(-1, >=, 1);
	PCU_ASSERT_OPERATOR_INT(a, ==, b);
	PCU_ASSERT_OPERATOR_INT(a, ^, b);
	PCU_ASSERT_OPERATOR_INT(a, ^, b+1);
	PCU_ASSERT_OPERATOR_INT(a, &, b);
	PCU_ASSERT_OPERATOR_INT(a, &, ~b);
	PCU_ASSERT_OPERATOR_INT(a, |, b);
	b++;
	PCU_ASSERT_OPERATOR_INT(a, ==, b);
	b = 0;
	PCU_ASSERT_OPERATOR_INT(a, &, b);
	PCU_ASSERT_OPERATOR_INT(0, |, b);
	PCU_ASSERT_OPERATOR_INT(0x10 <= a, &&, a < 0x100);
	PCU_ASSERT_OPERATOR_INT(a < 0x10, ||, 0x100 < a);
	PCU_ASSERT_OPERATOR_INT_FATAL(0, <, 1);
	PCU_ASSERT_OPERATOR_INT_FATAL(0, ==, 1);
	PCU_ASSERT_OPERATOR_INT(0, >, 1);
}

static void test_assert_operator_dbl(void)
{
	double a, b;
	a = 100.0;
	b = 100.0;
	PCU_ASSERT_OPERATOR_DOUBLE(0, <, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(0, <=, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(0, >, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(0, >=, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, <, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, <=, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, >, 1);
	PCU_ASSERT_OPERATOR_DOUBLE(-1, >=, 1);

	PCU_ASSERT_OPERATOR_DOUBLE(a, ==, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, <, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, >, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, <, b + 0.01);
	PCU_ASSERT_OPERATOR_DOUBLE(a, >, b + 0.01);

	PCU_ASSERT_OPERATOR_DOUBLE_FATAL(a, ==, b);
	PCU_ASSERT_OPERATOR_DOUBLE_FATAL(a, <, b);
	PCU_ASSERT_OPERATOR_DOUBLE(a, <, b);
}

static void test_assert_return(void)
{
	PCU_ASSERT_RETURN(0);
	PCU_ASSERT(0);
}

static void test_assert_true_return(void)
{
	PCU_ASSERT_TRUE_RETURN(0);
	PCU_ASSERT_TRUE(0);
}


static void test_assert_false_return(void)
{
	PCU_ASSERT_FALSE_RETURN(1);
	PCU_ASSERT_FALSE(1);
}


static void test_assert_equal_return(void)
{
	PCU_ASSERT_EQUAL_RETURN(0, 1);
	PCU_ASSERT_EQUAL(0, 1);
}


static void test_assert_not_equal_return(void)
{
	PCU_ASSERT_NOT_EQUAL_RETURN(0, 0);
	PCU_ASSERT_NOT_EQUAL(0, 0);
}


static void test_assert_ptr_equal_return(void)
{
	PCU_ASSERT_PTR_EQUAL_RETURN(0, __FILE__);
	PCU_ASSERT_PTR_EQUAL(0, __FILE__);
}


static void test_assert_ptr_not_equal_return(void)
{
	PCU_ASSERT_PTR_NOT_EQUAL_RETURN(0, 0);
	PCU_ASSERT_PTR_NOT_EQUAL(0, 0);
}


static void test_assert_ptr_null_return(void)
{
	PCU_ASSERT_PTR_NULL_RETURN(__FILE__);
	PCU_ASSERT_PTR_NULL(__FILE__);
}


static void test_assert_ptr_not_null_return(void)
{
	PCU_ASSERT_PTR_NOT_NULL_RETURN(0);
	PCU_ASSERT_PTR_NOT_NULL(0);
}


static void test_assert_str_equal_return(void)
{
	PCU_ASSERT_STRING_EQUAL_RETURN(__FILE__, __FILE__+1);
	PCU_ASSERT_STRING_EQUAL(__FILE__, __FILE__+1);
}


static void test_assert_str_not_equal_return(void)
{
	PCU_ASSERT_STRING_NOT_EQUAL_RETURN(__FILE__, __FILE__);
	PCU_ASSERT_STRING_NOT_EQUAL(__FILE__, __FILE__);
}


static void test_assert_nstr_equal_return(void)
{
	PCU_ASSERT_NSTRING_EQUAL_RETURN(__FILE__, __FILE__+1, 5);
	PCU_ASSERT_NSTRING_EQUAL(__FILE__, __FILE__+1, 5);
}


static void test_assert_nstr_not_equal_return(void)
{
	PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(__FILE__, __FILE__, 5);
	PCU_ASSERT_NSTRING_NOT_EQUAL(__FILE__, __FILE__, 5);
}


#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
static void test_assert_strw_equal_return(void)
{
	PCU_ASSERT_STRINGW_EQUAL_RETURN(WIDEN(__FILE__), WIDEN(__FILE__)+1);
	PCU_ASSERT_STRINGW_EQUAL(WIDEN(__FILE__), WIDEN(__FILE__)+1);
}


static void test_assert_strw_not_equal_return(void)
{
	PCU_ASSERT_STRINGW_NOT_EQUAL_RETURN(WIDEN(__FILE__), WIDEN(__FILE__));
	PCU_ASSERT_STRINGW_NOT_EQUAL(WIDEN(__FILE__), WIDEN(__FILE__));
}


static void test_assert_nstrw_equal_return(void)
{
	PCU_ASSERT_NSTRINGW_EQUAL_RETURN(WIDEN(__FILE__), WIDEN(__FILE__)+1, 5);
	PCU_ASSERT_NSTRINGW_EQUAL(WIDEN(__FILE__), WIDEN(__FILE__)+1, 5);
}


static void test_assert_nstrw_not_equal_return(void)
{
	PCU_ASSERT_NSTRINGW_NOT_EQUAL_RETURN(WIDEN(__FILE__), WIDEN(__FILE__), 5);
	PCU_ASSERT_NSTRINGW_NOT_EQUAL(WIDEN(__FILE__), WIDEN(__FILE__), 5);
}


static void test_assert_dbl_equal_return(void)
{
	PCU_ASSERT_DOUBLE_EQUAL_RETURN(0.0, 1.5, 0.0);
	PCU_ASSERT_DOUBLE_EQUAL(0.0, 1.5, 0.0);
}


static void test_assert_dbl_not_equal_return(void)
{
	PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(1.0, 1.0, 0.0);
	PCU_ASSERT_DOUBLE_NOT_EQUAL(1.0, 1.0, 0.0);
}


static void test_fail0_return(void)
{
	PCU_FAIL0_RETURN("test_msg0: 4");
	PCU_FAIL0("test_msg0: 4");
}


static void test_fail1_return(void)
{
	PCU_FAIL1_RETURN("test_msg1: %d", 4);
	PCU_FAIL1("test_msg1: %d", 5);
}


static void test_fail2_return(void)
{
	PCU_FAIL2_RETURN("test_msg2: %d,%#x", 4, 2);
	PCU_FAIL2("test_msg2: %d,%#x", 5, 2);
}


static void test_fail3_return(void)
{
	PCU_FAIL3_RETURN("test_msg3: %d,%#x, %c", 4, 2, '3');
	PCU_FAIL3("test_msg3: %d,%#x, %c", 5, 2, '3');
}


static void test_fail4_return(void)
{
	PCU_FAIL4_RETURN("test_msg4: %d,%#x, %c, %04d", 4, 2, '3', 4);
	PCU_FAIL4("test_msg4: %d,%#x, %c, %04d", 5, 2, '3', 4);
}


static void test_fail5_return(void)
{
	PCU_FAIL5_RETURN("test_msg5: %d,%#x, %c, %04d, %s", 4, 2, '3', 4, "5");
	PCU_FAIL5("test_msg5: %d,%#x, %c, %04d, %s", 5, 2, '3', 4, "5");
}


static void test_fail6_return(void)
{
	PCU_FAIL6_RETURN("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 4, 2, '3', 4, "5", 6);
	PCU_FAIL6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 5, 2, '3', 4, "5", 6);
}


static void test_fail7_return(void)
{
	PCU_FAIL7_RETURN("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 4, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 5, 2, '3', 4, "5", 6, 7);
}


static void test_fail8_return(void)
{
	PCU_FAIL8_RETURN("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 4, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 5, 2, '3', 4, "5", 6, 7, 8);
}


static void test_fail9_return(void)
{
	PCU_FAIL9_RETURN("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 4, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 5, 2, '3', 4, "5", 6, 7, 8, -9);
}


static void test_fail_return(void)
{
	PCU_FAIL_RETURN("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 4, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 5, 2, '3', 4, "5", 6, 7, 8, -9, 10);
}

static void test_failw_return(void)
{
	PCU_FAILW_RETURN(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 4, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
	PCU_FAILW(L"test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 5, 2, L'3', 4, L"5", 6, 7, 8, -9, 10);
}


static void test_assert_operator_return(void)
{
	PCU_ASSERT_OPERATOR_RETURN(-1, <, 1);
	PCU_ASSERT_OPERATOR(0, ==, 1);
}

static void test_assert_operator_int_return(void)
{
	PCU_ASSERT_OPERATOR_INT_RETURN(-1, >, 1);
	PCU_ASSERT_OPERATOR_INT(0, ==, 1);
}

static void test_assert_operator_dbl_return(void)
{
	PCU_ASSERT_OPERATOR_DOUBLE_RETURN(-1.1, >, 1.1);
	PCU_ASSERT_OPERATOR_DOUBLE(0, ==, 1);
}



PCU_Suite *AssertTest_suite(void)
{
	static PCU_Test tests[] = {
		{ "test_assert"                , test_assert                } ,
		{ "test_assert_true"           , test_assert_true           } ,
		{ "test_assert_false"          , test_assert_false          } ,
		{ "test_assert_equal"          , test_assert_equal          } ,
		{ "test_assert_not_equal"      , test_assert_not_equal      } ,
		{ "test_assert_ptr_equal"      , test_assert_ptr_equal      } ,
		{ "test_assert_ptr_not_equal"  , test_assert_ptr_not_equal  } ,
		{ "test_assert_ptr_null"       , test_assert_ptr_null       } ,
		{ "test_assert_ptr_not_null"   , test_assert_ptr_not_null   } ,
		{ "test_assert_str_equal"      , test_assert_str_equal      } ,
		{ "test_assert_str_not_equal"  , test_assert_str_not_equal  } ,
		{ "test_assert_nstr_equal"     , test_assert_nstr_equal     } ,
		{ "test_assert_nstr_not_equal" , test_assert_nstr_not_equal } ,
		{ "test_assert_strw_equal"     , test_assert_strw_equal     } ,
		{ "test_assert_strw_not_equal" , test_assert_strw_not_equal } ,
		{ "test_assert_nstrw_equal"    , test_assert_nstrw_equal    } ,
		{ "test_assert_nstrw_not_equal", test_assert_nstrw_not_equal} ,
		{ "test_assert_dbl_equal"      , test_assert_dbl_equal      } ,
		{ "test_assert_dbl_not_equal"  , test_assert_dbl_not_equal  } ,
		{ "test_msg0"                  , test_msg0                  } ,
		{ "test_msg1"                  , test_msg1                  } ,
		{ "test_msg2"                  , test_msg2                  } ,
		{ "test_msg3"                  , test_msg3                  } ,
		{ "test_msg4"                  , test_msg4                  } ,
		{ "test_msg5"                  , test_msg5                  } ,
		{ "test_msg6"                  , test_msg6                  } ,
		{ "test_msg7"                  , test_msg7                  } ,
		{ "test_msg8"                  , test_msg8                  } ,
		{ "test_msg9"                  , test_msg9                  } ,
		{ "test_msg"                   , test_msg                   } ,
		{ "test_msgw"                  , test_msgw                  } ,
		{ "test_assert_setup_err"      , test_assert_dummy          } ,
		{ "test_assert_teardown_err"   , test_assert_dummy          } ,
		{ "test_skip"                  , test_skip, -1              } ,
		{ "test_assert_operator"       , test_assert_operator       } ,
		{ "test_assert_operator_int"   , test_assert_operator_int   } ,
		{ "test_assert_operator_dbl"   , test_assert_operator_dbl   } ,
	};
	static PCU_Suite suite = { "AssertTest", tests, sizeof tests / sizeof *tests, setup, teardown };
	return &suite;
}

PCU_Suite *AssertReturnTest_suite(void)
{
	static PCU_Test tests[] = {
		{ "test_assert_return"                , test_assert_return                } , 
		{ "test_assert_true_return"           , test_assert_true_return           } , 
		{ "test_assert_false_return"          , test_assert_false_return          } , 
		{ "test_assert_equal_return"          , test_assert_equal_return          } , 
		{ "test_assert_not_equal_return"      , test_assert_not_equal_return      } , 
		{ "test_assert_ptr_equal_return"      , test_assert_ptr_equal_return      } , 
		{ "test_assert_ptr_not_equal_return"  , test_assert_ptr_not_equal_return  } , 
		{ "test_assert_ptr_null_return"       , test_assert_ptr_null_return       } , 
		{ "test_assert_ptr_not_null_return"   , test_assert_ptr_not_null_return   } , 
		{ "test_assert_str_equal_return"      , test_assert_str_equal_return      } , 
		{ "test_assert_str_not_equal_return"  , test_assert_str_not_equal_return  } , 
		{ "test_assert_nstr_equal_return"     , test_assert_nstr_equal_return     } , 
		{ "test_assert_nstr_not_equal_return" , test_assert_nstr_not_equal_return } , 
		{ "test_assert_strw_equal_return"     , test_assert_strw_equal_return     } , 
		{ "test_assert_strw_not_equal_return" , test_assert_strw_not_equal_return } , 
		{ "test_assert_nstrw_equal_return"    , test_assert_nstrw_equal_return    } , 
		{ "test_assert_nstrw_not_equal_return", test_assert_nstrw_not_equal_return} , 
		{ "test_assert_dbl_equal_return"      , test_assert_dbl_equal_return      } , 
		{ "test_assert_dbl_not_equal_return"  , test_assert_dbl_not_equal_return  } , 
		{ "test_fail0_return"                 , test_fail0_return                 } , 
		{ "test_fail1_return"                 , test_fail1_return                 } , 
		{ "test_fail2_return"                 , test_fail2_return                 } , 
		{ "test_fail3_return"                 , test_fail3_return                 } , 
		{ "test_fail4_return"                 , test_fail4_return                 } , 
		{ "test_fail5_return"                 , test_fail5_return                 } , 
		{ "test_fail6_return"                 , test_fail6_return                 } , 
		{ "test_fail7_return"                 , test_fail7_return                 } , 
		{ "test_fail8_return"                 , test_fail8_return                 } , 
		{ "test_fail9_return"                 , test_fail9_return                 } , 
		{ "test_fail_return"                  , test_fail_return                  } , 
		{ "test_failw_return"                 , test_failw_return                 } , 
		{ "test_assert_operator_return"       , test_assert_operator_return       } , 
		{ "test_assert_operator_int_return"   , test_assert_operator_int_return   } , 
		{ "test_assert_operator_dbl_return"   , test_assert_operator_dbl_return   } , 
	};
	static PCU_Suite suite = { "AssertReturnTest", tests, sizeof tests / sizeof *tests };
	return &suite;
}
