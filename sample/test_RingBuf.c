#include "PCUnit/PCUnit.h"
#include "RingBuf.h"
#include <stdio.h>

#define RB_BUF_SIZE		256
static RingBuf ringbuf;
static RingBuf *rb = &ringbuf;
static unsigned char rb_buffer[RB_BUF_SIZE];

static int initialize(void)
{
	/* Suiteの最初に呼ばれる */
	/* printf("initialize: %s\n", PCU_suite_name()); */
	return 0;
}

static int cleanup(void)
{
	/* Suiteの最後に呼ばれる */
	/* printf("cleanup   : %s\n", PCU_suite_name()); */
	return 0;
}

static int setup(void)
{
	/* test_rb_xxx毎に呼ばれる */
	RingBuf_init(rb, rb_buffer, sizeof rb_buffer);
	/* printf("  setup   : %s\n", PCU_test_name()); */
	return 0;
}

static int teardown(void)
{
	/* test_rb_xxx毎に呼ばれる */
	/* printf("  teardown: %s\n", PCU_test_name()); */
	return 0;
}

static void test_rb_init(void)
{
	PCU_ASSERT_EQUAL(0, RingBuf_size(rb));
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_maxsize(rb));
	PCU_ASSERT_TRUE(RingBuf_empty(rb));
	PCU_ASSERT_FALSE(RingBuf_full(rb));
}

static void test_rb_push(void)
{
	int i;
	int ret;
	for (i = 0; i < RingBuf_maxsize(rb); i++) {
		ret = RingBuf_push(rb, (unsigned char)i);
		PCU_ASSERT_TRUE(ret);
		PCU_ASSERT_EQUAL(i + 1, RingBuf_size(rb));
	}
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_size(rb));
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_maxsize(rb));
	PCU_ASSERT_FALSE(RingBuf_empty(rb));
	PCU_ASSERT_TRUE(RingBuf_full(rb));
	ret = RingBuf_push(rb, (unsigned char)i);
	PCU_ASSERT_FALSE(ret);
}

static void test_rb_pop(void)
{
	int i;
	int ret;
	for (i = 0; i < RingBuf_maxsize(rb); i++) {
		ret = RingBuf_push(rb, (unsigned char)i);
		PCU_ASSERT_TRUE(ret);
		PCU_ASSERT_EQUAL(i + 1, RingBuf_size(rb));
	}
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_size(rb));
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_maxsize(rb));
	PCU_ASSERT_FALSE(RingBuf_empty(rb));
	PCU_ASSERT_TRUE(RingBuf_full(rb));
	ret = RingBuf_push(rb, (unsigned char)i);
	PCU_ASSERT_FALSE(ret);

	for (i = 0; !RingBuf_empty(rb); i++) {
		PCU_ASSERT_EQUAL(i, RingBuf_pop(rb));
		PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1 - i - 1, RingBuf_size(rb));
	}
	PCU_ASSERT_EQUAL(0, RingBuf_size(rb));
	PCU_ASSERT_EQUAL(RB_BUF_SIZE - 1, RingBuf_maxsize(rb));
	PCU_ASSERT_TRUE(RingBuf_empty(rb));
	PCU_ASSERT_FALSE(RingBuf_full(rb));


	for (i = 0; i < RingBuf_maxsize(rb) / 2; i++) {
		ret = RingBuf_push(rb, (unsigned char)i);
		PCU_ASSERT_TRUE(ret);
		PCU_ASSERT_EQUAL(i + 1, RingBuf_size(rb));
	}
	PCU_ASSERT_EQUAL((RB_BUF_SIZE - 1) / 2, RingBuf_size(rb));
	PCU_ASSERT_FALSE(RingBuf_empty(rb));

	for (i = 0; !RingBuf_empty(rb); i++) {
		PCU_ASSERT_EQUAL(i, RingBuf_pop(rb));
		PCU_ASSERT_EQUAL((RB_BUF_SIZE - 1) / 2 - i - 1, RingBuf_size(rb));
	}
	PCU_ASSERT_EQUAL(0, RingBuf_size(rb));
	PCU_ASSERT_TRUE(RingBuf_empty(rb));
	PCU_ASSERT_FALSE(RingBuf_full(rb));
}


PCU_Suite *RingBufTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_rb_init),
		PCU_TEST(test_rb_push),
		PCU_TEST(test_rb_pop),
	};
	static PCU_Suite suite = { "RingBufTest", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup };
	return &suite;
}

