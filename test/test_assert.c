#include <stdio.h>
#undef PCU_NO_LIBC
#include "../PCUnit/PCUnit.h"

#define PCU_snprintf		snprintf

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
	PCU_MSG0("test_msg0: 1");
	PCU_MSG0("test_msg0: 2");
	PCU_FAIL0("test_msg0: 3");
	PCU_FAIL0_FATAL("test_msg0: 4");
	PCU_FAIL0("test_msg0: 4");
}

static void test_msg1(void)
{
	PCU_MSG1("test_msg1: %d", 1);
	PCU_MSG1("test_msg1: %d", 2);
	PCU_FAIL1("test_msg1: %d", 3);
	PCU_FAIL1_FATAL("test_msg1: %d", 4);
	PCU_FAIL1("test_msg1: %d", 5);
}

static void test_msg2(void)
{
	PCU_MSG2("test_msg2: %d,%#x", 1, 2);
	PCU_MSG2("test_msg2: %d,%#x", 2, 2);
	PCU_FAIL2("test_msg2: %d,%#x", 3, 2);
	PCU_FAIL2_FATAL("test_msg2: %d,%#x", 4, 2);
	PCU_FAIL2("test_msg2: %d,%#x", 5, 2);
}

static void test_msg3(void)
{
	PCU_MSG3("test_msg3: %d,%#x, %c", 1, 2, '3');
	PCU_MSG3("test_msg3: %d,%#x, %c", 2, 2, '3');
	PCU_FAIL3("test_msg3: %d,%#x, %c", 3, 2, '3');
	PCU_FAIL3_FATAL("test_msg3: %d,%#x, %c", 4, 2, '3');
	PCU_FAIL3("test_msg3: %d,%#x, %c", 5, 2, '3');
}

static void test_msg4(void)
{
	PCU_MSG4("test_msg4: %d,%#x, %c, %04d", 1, 2, '3', 4);
	PCU_MSG4("test_msg4: %d,%#x, %c, %04d", 2, 2, '3', 4);
	PCU_FAIL4("test_msg4: %d,%#x, %c, %04d", 3, 2, '3', 4);
	PCU_FAIL4_FATAL("test_msg4: %d,%#x, %c, %04d", 4, 2, '3', 4);
	PCU_FAIL4("test_msg4: %d,%#x, %c, %04d", 5, 2, '3', 4);
}

static void test_msg5(void)
{
	PCU_MSG5("test_msg5: %d,%#x, %c, %04d, %s", 1, 2, '3', 4, "5");
	PCU_MSG5("test_msg5: %d,%#x, %c, %04d, %s", 2, 2, '3', 4, "5");
	PCU_FAIL5("test_msg5: %d,%#x, %c, %04d, %s", 3, 2, '3', 4, "5");
	PCU_FAIL5_FATAL("test_msg5: %d,%#x, %c, %04d, %s", 4, 2, '3', 4, "5");
	PCU_FAIL5("test_msg5: %d,%#x, %c, %04d, %s", 5, 2, '3', 4, "5");
}

static void test_msg6(void)
{
	PCU_MSG6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 1, 2, '3', 4, "5", 6);
	PCU_MSG6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 2, 2, '3', 4, "5", 6);
	PCU_FAIL6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 3, 2, '3', 4, "5", 6);
	PCU_FAIL6_FATAL("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 4, 2, '3', 4, "5", 6);
	PCU_FAIL6("test_msg6: %d,%#x, %c, %04d, %s, 0x%08X", 5, 2, '3', 4, "5", 6);
}

static void test_msg7(void)
{
	PCU_MSG7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 1, 2, '3', 4, "5", 6, 7);
	PCU_MSG7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 2, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 3, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7_FATAL("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 4, 2, '3', 4, "5", 6, 7);
	PCU_FAIL7("test_msg7: %d,%#x, %c, %04d, %s, 0x%08X, %+d", 5, 2, '3', 4, "5", 6, 7);
}

static void test_msg8(void)
{
	PCU_MSG8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 1, 2, '3', 4, "5", 6, 7, 8);
	PCU_MSG8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 2, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 3, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8_FATAL("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 4, 2, '3', 4, "5", 6, 7, 8);
	PCU_FAIL8("test_msg8: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,", 5, 2, '3', 4, "5", 6, 7, 8);
}

static void test_msg9(void)
{
	PCU_MSG9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 1, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_MSG9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 2, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 3, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9_FATAL("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 4, 2, '3', 4, "5", 6, 7, 8, -9);
	PCU_FAIL9("test_msg9: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d", 5, 2, '3', 4, "5", 6, 7, 8, -9);
}

static void test_msg(void)
{
#if __STDC_VERSION__ >= 199901L
	PCU_MSG("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_MSG("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 2, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 3, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL_FATAL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 4, 2, '3', 4, "5", 6, 7, 8, -9, 10);
	PCU_FAIL("test_msg: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 5, 2, '3', 4, "5", 6, 7, 8, -9, 10);
#endif
}

static void test_assert_dummy(void)
{
	PCU_ASSERT(0);
}

int assert_tests_setup(void)
{
	if (strcmp(PCU_test_name(), "test_assert_setup_err") == 0) {
		return -1;
	}
	return 0;
}

int assert_tests_teardown(void)
{
	if (strcmp(PCU_test_name(), "test_assert_teardown_err") == 0) {
		return -2;
	}
	return 0;
}

static void test_long_long(void)
{
	long long a = 0x100000000;
	long long b = 0x100000000;

	PCU_ASSERT_EQUAL(a, b);
	b++;
	PCU_ASSERT_EQUAL(a, b);
	b -= 2;
	PCU_ASSERT_EQUAL(a, b);
	a--;
	PCU_ASSERT_EQUAL(a, b);
	a--;
	PCU_ASSERT_EQUAL(a, b);
}

static void test_skip(void)
{
	PCU_ASSERT(0);
}


static void test_assert_operator(void)
{
	int a, b;
	a = 0x100;
	b = 0x100;
	PCU_ASSERT_OPERATOR(0, <, 1);
	PCU_ASSERT_OPERATOR(0, <=, 1);
	PCU_ASSERT_OPERATOR(0, >, 1);
	PCU_ASSERT_OPERATOR(0, >=, 1);
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


PCU_Test assert_tests[] = {
	{ "test_assert"                , test_assert                } ,
	{ "test_assert_true"           , test_assert_true           } ,
	{ "test_assert_false"          , test_assert_false          } ,
	{ "test_assert_equal"          , test_assert_equal          } ,
	{ "test_assert_not_equal"      , test_assert_not_equal      } ,
	{ "test_assert_ptr_equal"      , test_assert_ptr_equal      } ,
	{ "test_assert_ptr_not_equal"  , test_assert_ptr_not_equal  } ,
	{ "test_assert_str_equal"      , test_assert_str_equal      } ,
	{ "test_assert_str_not_equal"  , test_assert_str_not_equal  } ,
	{ "test_assert_nstr_equal"     , test_assert_nstr_equal     } ,
	{ "test_assert_nstr_not_equal" , test_assert_nstr_not_equal } ,
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
	{ "test_assert_setup_err"      , test_assert_dummy          } ,
	{ "test_assert_teardown_err"   , test_assert_dummy          } ,
	{ "test_long_long"             , test_long_long             } ,
	{ "test_skip"                  , test_skip, -1              } ,
	{ "test_assert_operator"       , test_assert_operator       } ,
	PCU_NULL,
};
