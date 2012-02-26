#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../PCUnit/PCUnit.h"
#undef PCU_MESSAGE_BUF_SIZE
#define PCU_MESSAGE_BUF_SIZE	256
const char *PCU_format_test(const char *format, ...);


void test_format(void)
{
	const char *bufp = 0;
	char sp_buf[PCU_MESSAGE_BUF_SIZE];
	int n;

	snprintf(sp_buf, sizeof sp_buf, "hogehoge");
	bufp = PCU_format_test(         "hogehoge");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%s", "piyopiyo");
	bufp = PCU_format_test(         "%s", "piyopiyo");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "foo%%bar%s", "hoge");
	bufp = PCU_format_test(         "foo%%bar%s", "hoge");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	bufp = PCU_format_test(         "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	bufp = PCU_format_test(         "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	bufp = PCU_format_test(         "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	bufp = PCU_format_test(         "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	bufp = PCU_format_test(         "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	bufp = PCU_format_test(         "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	bufp = PCU_format_test(         "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	bufp = PCU_format_test(         "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	bufp = PCU_format_test(         "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	bufp = PCU_format_test(         "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	bufp = PCU_format_test(         "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	bufp = PCU_format_test(         "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	bufp = PCU_format_test(         "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	n = 8;
	snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	bufp = PCU_format_test(         "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	n = 0;
	snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	bufp = PCU_format_test(         "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%c, %c, %c, %c", 'a', '#', '\\', '9');
	bufp = PCU_format_test(         "%c, %c, %c, %c", 'a', '#', '\\', '9');
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);


	snprintf(sp_buf, sizeof sp_buf, "%p, %p, %p, %p", (void *)1, (void *)bufp, (void *)sp_buf, (void *)1);
	bufp = PCU_format_test(         "%p, %p, %p, %p", 1, bufp, sp_buf, 1);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

}

void test_format_shorten(void)
{
	const char *bufp = 0;
	char sp_buf[PCU_MESSAGE_BUF_SIZE];
#define STR	"0123456789abcdef"\
			"1123456789abcdef"\
			"2123456789abcdef"\
			"3123456789abcdef"\
			"4123456789abcdef"\
			"5123456789abcdef"\
			"6123456789abcdef"\
			"7123456789abcdef"\
			"8123456789abcdef"\
			"9123456789abcdef"\
			"a123456789abcdef"\
			"b123456789abcdef"\
			"c123456789abcdef"\
			"d123456789abcdef"\
			"e123456789abcdef"

#define STR_0	STR "f123456789abcd01"
#define STR_S	STR "f123456789abcd%s"
#define STR_D	STR "f123456789abcd%d"
#define STR_X	STR "f123456789abcd%x"

	snprintf(sp_buf, sizeof sp_buf, STR_0);
	bufp = PCU_format_test(STR_0);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_S, "xy");
	bufp = PCU_format_test(STR_S, "xy");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_D, 12);
	bufp = PCU_format_test(STR_D, 12);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_X, 0xab);
	bufp = PCU_format_test(STR_X, 0xab);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

}


PCU_Suite *FormatTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_format),
		PCU_TEST(test_format_shorten),
	};
	static PCU_Suite suite = { "FormatTest", tests, sizeof tests / sizeof tests[0] };
	return &suite;
}
