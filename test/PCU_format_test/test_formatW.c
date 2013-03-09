#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <locale.h>

#include "../../PCUnit/PCUnit.h"
#define PCU_FORMAT_BUFSIZE	256

static int initialize(void)
{
	setlocale(LC_CTYPE, "ja_JP.UTF-8");
	return 0;
}

void test_formatW(void)
{
	const char *bufp = 0;
	char sp_buf[PCU_FORMAT_BUFSIZE];
	int n;

	snprintf(sp_buf, sizeof sp_buf,  "hogehoge");
	bufp = PCU_formatW(             L"hogehoge");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%s", "piyopiyo");
	bufp = PCU_formatW(             L"%ls", L"piyopiyo");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "foo%%bar%s", "hoge");
	bufp = PCU_formatW(             L"foo%%bar%ls", L"hoge");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	bufp = PCU_formatW(             L"%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	bufp = PCU_formatW(             L"%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	bufp = PCU_formatW(             L"%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	bufp = PCU_formatW(             L"\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	bufp = PCU_formatW(             L"\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	bufp = PCU_formatW(             L"%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	bufp = PCU_formatW(             L"%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	bufp = PCU_formatW(             L"%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	bufp = PCU_formatW(             L"%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	bufp = PCU_formatW(             L"%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	bufp = PCU_formatW(             L"%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	bufp = PCU_formatW(             L"% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	bufp = PCU_formatW(             L"% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	n = 8;
	snprintf(sp_buf, sizeof sp_buf,  "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	bufp = PCU_formatW(             L"\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	n = 0;
	snprintf(sp_buf, sizeof sp_buf,  "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	bufp = PCU_formatW(             L"\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf,  "%c, %c, %c, %c", 'a', '#', '\\', '9');
	bufp = PCU_formatW(             L"%c, %c, %c, %c", 'a', '#', '\\', '9');
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);


	snprintf(sp_buf, sizeof sp_buf,  "%p, %p, %p, %p", (void *)1, (void *)bufp, (void *)sp_buf, (void *)1);
	bufp = PCU_formatW(             L"%p, %p, %p, %p", 1, bufp, sp_buf, 1);
	PCU_MESSAGE(sp_buf);
	PCU_MESSAGE(bufp);

	snprintf(sp_buf, sizeof sp_buf,  "あいうえお%s",   "かきくけこ");
	bufp = PCU_formatW(             L"あいうえお%ls", L"かきくけこ");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, %d, %d, %u", CHAR_MAX, CHAR_MIN, SCHAR_MAX, SCHAR_MIN, UCHAR_MAX, UCHAR_MAX);
	bufp = PCU_formatW(            L"%d, %d, %d, %d, %d, %u", CHAR_MAX, CHAR_MIN, SCHAR_MAX, SCHAR_MIN, UCHAR_MAX, UCHAR_MAX);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, %u", SHRT_MAX, SHRT_MIN, USHRT_MAX, USHRT_MAX);
	bufp = PCU_formatW(            L"%d, %d, %d, %u", SHRT_MAX, SHRT_MIN, USHRT_MAX, USHRT_MAX);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, %u", INT_MAX, INT_MIN, UINT_MAX, UINT_MAX);
	bufp = PCU_formatW(            L"%d, %d, %d, %u", INT_MAX, INT_MIN, UINT_MAX, UINT_MAX);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%ld, %ld, %ld, %lu", LONG_MAX, LONG_MIN, ULONG_MAX, ULONG_MAX);
	bufp = PCU_formatW(            L"%ld, %ld, %ld, %lu", LONG_MAX, LONG_MIN, ULONG_MAX, ULONG_MAX);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, "%lld, %lld, %lld, %llu", LLONG_MAX, LLONG_MIN, ULLONG_MAX, ULLONG_MAX);
	bufp = PCU_formatW(            L"%lld, %lld, %lld, %llu", LLONG_MAX, LLONG_MIN, ULLONG_MAX, ULLONG_MAX);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

}

void test_formatW_shorten(void)
{
	const char *bufp = 0;
	char sp_buf[PCU_FORMAT_BUFSIZE];
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

#define LSTR	L"0123456789abcdef"\
				L"1123456789abcdef"\
				L"2123456789abcdef"\
				L"3123456789abcdef"\
				L"4123456789abcdef"\
				L"5123456789abcdef"\
				L"6123456789abcdef"\
				L"7123456789abcdef"\
				L"8123456789abcdef"\
				L"9123456789abcdef"\
				L"a123456789abcdef"\
				L"b123456789abcdef"\
				L"c123456789abcdef"\
				L"d123456789abcdef"\
				L"e123456789abcdef"

#define LSTR_0	LSTR L"f123456789abcd01"
#define LSTR_S	LSTR L"f123456789abcd%s"
#define LSTR_D	LSTR L"f123456789abcd%d"
#define LSTR_X	LSTR L"f123456789abcd%x"

	snprintf(sp_buf, sizeof sp_buf, STR_0);
	bufp = PCU_formatW(LSTR_0);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_S, "xy");
	bufp = PCU_formatW(LSTR_S, "xy");
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_D, 12);
	bufp = PCU_formatW(LSTR_D, 12);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

	snprintf(sp_buf, sizeof sp_buf, STR_X, 0xab);
	bufp = PCU_formatW(LSTR_X, 0xab);
	PCU_ASSERT_STRING_EQUAL(sp_buf, bufp);

}


PCU_Suite *FormatWTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_formatW),
		PCU_TEST(test_formatW_shorten),
	};
	static PCU_Suite suite = { "FormatWTest", tests, sizeof tests / sizeof tests[0], 0, 0, initialize, 0 };
	return &suite;
}
