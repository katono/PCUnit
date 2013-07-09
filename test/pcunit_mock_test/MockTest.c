#include "PCUnit/PCUnit.h"
#include "mock_target.h"
#include <string.h>

static int setup(void)
{
	mock_target_init();
	return 0;
}

static int teardown(void)
{
	mock_target_verify();
	return 0;
}


static void test_f_int_args(void)
{
	f_int_args_Expectation e[1] = {{0}};
	e[0].expected.a = 1;
	e[0].expected.b = 2;
	e[0].expected.c = 3;
	e[0].expected.d = 4;
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
}

static void test_f_int_args_all_params_ignored(void)
{
	f_int_args_Expectation e[1] = {{0}};
	memset(&e[0].ignored, 0xFF, sizeof e[0].ignored);
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
}

static void test_f_int_args_param_ignored(void)
{
	f_int_args_Expectation e[1] = {{0}};
	e[0].expected.a = 1;
	e[0].ignored.b = 1;
	e[0].expected.c = 3;
	e[0].ignored.d = 1;
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 1000, 3, 1000));
}

static void test_f_int_args_2times(void)
{
	f_int_args_Expectation e[2] = {{0}};
	e[0].expected.a = 1;
	e[0].expected.b = 2;
	e[0].expected.c = 3;
	e[0].expected.d = 4;
	e[0].retval = 5;
	e[1].expected.a = 10;
	e[1].expected.b = 20;
	e[1].expected.c = 30;
	e[1].expected.d = 40;
	e[1].retval = 50;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	PCU_ASSERT_EQUAL(50, f_int_args(10, 20, 30, 40));
}

static void test_f_const_char_ptr(void)
{
	f_const_char_ptr_Expectation e[1] = {{0}};
	e[0].expected.a = "hoge";
	e[0].retval = "foo";
	f_const_char_ptr_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_STRING_EQUAL("foo", f_const_char_ptr("hoge"));
}

static void test_f_void(void)
{
	f_void_omitted_Expectation e[1] = {{0}};
	f_void_omitted_expect(e, sizeof e / sizeof e[0]);
	f_void_omitted();
}

static void test_f_STRUCT_t_ptr(void)
{
	STRUCT_t st1 = {100};
	STRUCT_t st2 = {10};
	STRUCT_t st3 = {10};
	f_STRUCT_t_ptr_Expectation e[1] = {{0}};
	e[0].expected.a = &st1;
	e[0].retval = &st2;
	f_STRUCT_t_ptr_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_MEMORY_EQUAL(&st3, f_STRUCT_t_ptr(&st1), sizeof(STRUCT_t), 1);
}


static int f_int_args_callback(int a, int b, int c, int d)
{
	const int expected_a[] = {1, 10, 100, 1000};
	const int expected_b[] = {2, 20, 200, 2000};
	const int expected_c[] = {3, 30, 300, 3000};
	const int expected_d[] = {4, 40, 400, 4000};
	const int retval[]     = {5, 50, 500, 5000};
	PCU_ASSERT_EQUAL(expected_a[f_int_args_num_calls()], a);
	PCU_ASSERT_EQUAL(expected_b[f_int_args_num_calls()], b);
	PCU_ASSERT_EQUAL(expected_c[f_int_args_num_calls()], c);
	PCU_ASSERT_EQUAL(expected_d[f_int_args_num_calls()], d);
	return retval[f_int_args_num_calls()];
}

static void test_f_int_args_callback(void)
{
	f_int_args_set_callback(f_int_args_callback, 4);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	PCU_ASSERT_EQUAL(50, f_int_args(10, 20, 30, 40));
	PCU_ASSERT_EQUAL(500, f_int_args(100, 200, 300, 400));
	PCU_ASSERT_EQUAL(5000, f_int_args(1000, 2000, 3000, 4000));
}

static int *f_int_ptr_callback(int *a)
{
	const int ret_a[] = {1, 10, 100, 1000};
	PCU_ASSERT_PTR_NOT_NULL(a);
	*a = ret_a[f_int_ptr_num_calls()];
	return 0;
}

static void test_f_int_ptr_return_by_param_ptr(void)
{
	int a = 0;
	f_int_ptr_set_callback(f_int_ptr_callback, 4);
	f_int_ptr(&a);
	PCU_ASSERT_EQUAL(1, a);
	a = 0;
	f_int_ptr(&a);
	PCU_ASSERT_EQUAL(10, a);
	a = 0;
	f_int_ptr(&a);
	PCU_ASSERT_EQUAL(100, a);
	a = 0;
	f_int_ptr(&a);
	PCU_ASSERT_EQUAL(1000, a);
}

static void f_int_varg_callback(int *a, va_list ap)
{
	if (f_int_varg_num_calls() == 0) {
		PCU_ASSERT_EQUAL(1, va_arg(ap, int));
		PCU_ASSERT_EQUAL(2, va_arg(ap, int));
		PCU_ASSERT_EQUAL(3, va_arg(ap, int));
	} else {
		PCU_ASSERT_EQUAL(10, va_arg(ap, int));
		PCU_ASSERT_EQUAL(20, va_arg(ap, int));
		PCU_ASSERT_EQUAL(30, va_arg(ap, int));
		PCU_ASSERT_STRING_EQUAL("hoge", va_arg(ap, const char *));
	}
}

static void test_f_int_varg_callback(void)
{
	f_int_varg_set_callback(f_int_varg_callback, 2);
	f_int_varg(NULL, 1, 2, 3);
	f_int_varg(NULL, 10, 20, 30, "hoge");
}

static int f_int_args_callback_many_times(int a, int b, int c, int d)
{
	PCU_ASSERT_EQUAL(1, a);
	PCU_ASSERT_EQUAL(2, b);
	PCU_ASSERT_EQUAL(3, c);
	PCU_ASSERT_EQUAL(4, d);
	return 5;
}

static void test_f_int_args_callback_many_times(void)
{
	int i;
	f_int_args_set_callback(f_int_args_callback_many_times, -1);
	for (i = 0; i < 10000; i++) {
		PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	}
}


PCU_Suite *MockTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_f_int_args),
		PCU_TEST(test_f_int_args_all_params_ignored),
		PCU_TEST(test_f_int_args_param_ignored),
		PCU_TEST(test_f_int_args_2times),
		PCU_TEST(test_f_const_char_ptr),
		PCU_TEST(test_f_void),
		PCU_TEST(test_f_STRUCT_t_ptr),
		PCU_TEST(test_f_int_args_callback),
		PCU_TEST(test_f_int_ptr_return_by_param_ptr),
		PCU_TEST(test_f_int_varg_callback),
		PCU_TEST(test_f_int_args_callback_many_times),
	};
	static PCU_Suite suite = {
		"MockTest", tests, sizeof tests / sizeof tests[0], setup, teardown
	};
	return &suite;
}

