
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


static void test_fail1(void)
{
	PCU_MESSAGE("test_message");
	PCU_FAIL("test_fail");
}

static void test_fail2(void)
{
	PCU_MESSAGE(PCU_format("test_message: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10));
	PCU_FAIL(PCU_format("test_fail: %d,%#x, %c, %04d, %s, 0x%08X, %+d, %-8x,% d, %10d", 1, 2, '3', 4, "5", 6, 7, 8, -9, 10));
}


static PCU_Test tests[] = {
	PCU_TEST(test_fail1),
	PCU_TEST(test_fail2),
};

