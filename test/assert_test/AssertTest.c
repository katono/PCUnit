
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


static void test_assert_OK(void)
{
	PCU_ASSERT(1);
}

static void test_assert_FAIL(void)
{
	PCU_ASSERT(0);
	PCU_ASSERT(0);
}

static void test_assert_MSG(void)
{
	int a = 1;
	PCU_ASSERT_MESSAGE(a, PCU_format("%d", a));
	a = 0;
	PCU_ASSERT_MESSAGE(a, PCU_format("%d", a));
}

static void test_assert_true_OK(void)
{
	int a = 1;
	PCU_ASSERT_TRUE(a);
}

static void test_assert_true_FAIL(void)
{
	int a = 0;
	PCU_ASSERT_TRUE(a);
	PCU_ASSERT_TRUE(0);
}

static void test_assert_true_MSG(void)
{
	int a = 1;
	PCU_ASSERT_TRUE_MESSAGE(a, PCU_format("%d", a));
	a = 0;
	PCU_ASSERT_TRUE_MESSAGE(a, PCU_format("%d", a));
}

static void test_assert_false_OK(void)
{
	int a = 0;
	PCU_ASSERT_FALSE(a);
}

static void test_assert_false_FAIL(void)
{
	int a = 1;
	PCU_ASSERT_FALSE(a);
	PCU_ASSERT_FALSE(1);
}

static void test_assert_false_MSG(void)
{
	int a = 0;
	PCU_ASSERT_FALSE_MESSAGE(a, PCU_format("%d", a));
	a = 1;
	PCU_ASSERT_FALSE_MESSAGE(a, PCU_format("%d", a));
}

static PCU_Test tests[] = {
	PCU_TEST(test_assert_OK),
	PCU_TEST(test_assert_FAIL),
	PCU_TEST(test_assert_MSG),
	PCU_TEST(test_assert_true_OK),
	PCU_TEST(test_assert_true_FAIL),
	PCU_TEST(test_assert_true_MSG),
	PCU_TEST(test_assert_false_OK),
	PCU_TEST(test_assert_false_FAIL),
	PCU_TEST(test_assert_false_MSG),
};


