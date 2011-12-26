
static int initialize(void)
{
	return 0;
}

static int cleanup(void)
{
	return -1;
}

static int setup(void)
{
	return 0;
}

static int teardown(void)
{
	return 0;
}


static void test_assert_cleanup_err(void)
{
	PCU_ASSERT(1);
}


static PCU_Test tests[] = {
	PCU_TEST(test_assert_cleanup_err),
};

