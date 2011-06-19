#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef PCU_NO_STDLIB
#include "../PCUnit/include/PCUnit.h"
int PCU_snprintf4(char *buf, size_t size, const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4);
int PCU_snprintf5(char *buf, size_t size, const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);

#define PCU_snprintf		snprintf

void test_snprintf(void)
{
	static char buf[1024];
	static char sp_buf[1024];
	int n;
	unsigned long buf_ret;
	unsigned long sp_buf_ret;
	char *buf_endp;
	char *sp_buf_endp;
	int ret1;
	int ret2;

	ret1 = snprintf(sp_buf, sizeof sp_buf, "hogehoge");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "hogehoge", 0, 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%s", "piyopiyo");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%s", (unsigned long)"piyopiyo", 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "foo%%bar%s", "hoge");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "foo%%bar%s", (unsigned long)"hoge", 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, (unsigned long)"abcd");
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%s\", \"%9d\", \"%s\", \"%s\"", (unsigned long)"foo", 1234567890, (unsigned long)"bar", (unsigned long)"abcd");
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	ret2 = PCU_snprintf5(buf, sizeof buf,  "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	n = 8;
	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	n = 0;
	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%c, %c, %c, %c", 'a', '#', '\\', '9');
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%c, %c, %c, %c", 'a', '#', '\\', '9');
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);


	ret1 = snprintf(sp_buf, sizeof sp_buf, "%p, %p, %p, %p", (void *)0, (void *)buf, (void *)sp_buf, (void *)0);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%p, %p, %p, %p", 0, (unsigned long)buf, (unsigned long)sp_buf, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
	sp_buf_ret = strtoul(sp_buf, &sp_buf_endp, 16);
	buf_ret = strtoul(buf, &buf_endp, 16);
	PCU_ASSERT_EQUAL(sp_buf_ret, buf_ret);
	PCU_ASSERT_EQUAL(',', *sp_buf_endp);
	PCU_ASSERT_EQUAL(',', *buf_endp);
	sp_buf_endp++;
	buf_endp++;

	sp_buf_ret = strtoul(sp_buf_endp, &sp_buf_endp, 16);
	buf_ret = strtoul(buf_endp, &buf_endp, 16);
	PCU_ASSERT_EQUAL(sp_buf_ret, buf_ret);
	PCU_ASSERT_EQUAL(',', *sp_buf_endp);
	PCU_ASSERT_EQUAL(',', *buf_endp);
	sp_buf_endp++;
	buf_endp++;

	sp_buf_ret = strtoul(sp_buf_endp, &sp_buf_endp, 16);
	buf_ret = strtoul(buf_endp, &buf_endp, 16);
	PCU_ASSERT_EQUAL(sp_buf_ret, buf_ret);
	PCU_ASSERT_EQUAL(',', *sp_buf_endp);
	PCU_ASSERT_EQUAL(',', *buf_endp);
	sp_buf_endp++;
	buf_endp++;

	sp_buf_ret = strtoul(sp_buf_endp, &sp_buf_endp, 16);
	buf_ret = strtoul(buf_endp, &buf_endp, 16);
	PCU_ASSERT_EQUAL(sp_buf_ret, buf_ret);
	PCU_ASSERT_EQUAL('\0', *sp_buf_endp);
	PCU_ASSERT_EQUAL('\0', *buf_endp);
/*    PCU_MSG2("\n%s\n%s\n", sp_buf, buf);*/

}

void test_snprintf2(void)
{
	static char buf[8];
	static char sp_buf[8];
	int n;
	int ret1;
	int ret2;
	ret1 = snprintf(sp_buf, sizeof sp_buf, "hogehoge");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "hogehoge", 0, 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%s", "piyopiyo");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%s", (unsigned long)"piyopiyo", 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "foo%%bar%s", "hoge");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "foo%%bar%s", (unsigned long)"hoge", 0, 0, 0);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%hd, %ld, %ld, \"%8ld\"", 9999, 0L, -1L, -1234L);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, (unsigned long)"abcd");
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%s\", \"%9d\", \"%s\", \"%s\"", (unsigned long)"foo", 1234567890, (unsigned long)"bar", (unsigned long)"abcd");
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%#x, \"%#8x\", \"%#08X\", \"%#-10x\"", 0x1234, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%#x, \"%#8x\", \"%#08X\", \"%#010X\"", 0, 0x5678, 0xABCD, 0x12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%+d, \"%+8d\", \"%-+8d\", \"%+010d\"", 0, 0, -123, -1);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%+d, \"%+8d\", \"%+-8d\", \"%+10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	ret2 = PCU_snprintf5(buf, sizeof buf,  "% d, \"% 8d\", \"%- 8d\", \"% 010d\", \"% d\"", 0, 0, -123, -1, -2);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "% d, \"% 8d\", \"% -8d\", \"% 10d\"", 1234, 5678, 9876, 12345);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	n = 8;
	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%*d\", \"%*X\"", n, 5678, -n, 0xABCD);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	n = 0;
	ret1 = snprintf(sp_buf, sizeof sp_buf, "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	ret2 = PCU_snprintf4(buf, sizeof buf,  "\"%*d\", \"%*X\"", n, 1234, -n, 0xABCD);
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);

	ret1 = snprintf(sp_buf, sizeof sp_buf, "%c, %c, %c, %c", 'a', '#', '\\', '9');
	ret2 = PCU_snprintf4(buf, sizeof buf,  "%c, %c, %c, %c", 'a', '#', '\\', '9');
	PCU_ASSERT_EQUAL(ret1, ret2);
/*    printf("%s\n", buf);*/
	PCU_ASSERT_STRING_EQUAL(sp_buf, buf);
}

PCU_Test snprintf_tests[] = {
	{ "test_snprintf", test_snprintf },
	{ "test_snprintf2", test_snprintf2 },
	PCU_NULL,
};
