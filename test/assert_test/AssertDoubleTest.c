
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



static void test_assert_dbl_equal_OK(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.0;
	delta = 0.0;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);

	a = 100.0;
	b = 100.1;
	delta = 0.1;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);

}

static void test_assert_dbl_equal_FAIL(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.0999;
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_EQUAL(a, b, delta);
}

static void test_assert_dbl_equal_MSG(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.1000;
	PCU_ASSERT_DOUBLE_EQUAL_MESSAGE(a, b, delta, PCU_format("%g, %g", a, b));
	delta = 0.0999;
	PCU_ASSERT_DOUBLE_EQUAL_MESSAGE(a, b, delta, PCU_format("%g, %g", a, b));
}



static void test_assert_dbl_not_equal_OK(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.0999;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);

	a = 100.01;
	b = 100.0;
	delta = 0.001;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);

}

static void test_assert_dbl_not_equal_FAIL(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.1;
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
	PCU_ASSERT_DOUBLE_NOT_EQUAL(a, b, delta);
}

static void test_assert_dbl_not_equal_MSG(void)
{
	double a, b, delta;
	a = 100.0;
	b = 100.1;
	delta = 0.0999;
	PCU_ASSERT_DOUBLE_NOT_EQUAL_MESSAGE(a, b, delta, PCU_format("%g, %g", a, b));
	delta = 0.1000;
	PCU_ASSERT_DOUBLE_NOT_EQUAL_MESSAGE(a, b, delta, PCU_format("%g, %g", a, b));
}



static PCU_Test tests[] = {
	PCU_TEST(test_assert_dbl_equal_OK),
	PCU_TEST(test_assert_dbl_equal_FAIL),
	PCU_TEST(test_assert_dbl_equal_MSG),
	PCU_TEST(test_assert_dbl_not_equal_OK),
	PCU_TEST(test_assert_dbl_not_equal_FAIL),
	PCU_TEST(test_assert_dbl_not_equal_MSG),
};

