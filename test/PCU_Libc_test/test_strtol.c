#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../PCUnit/PCUnit.h"
long PCU_strtol(const char *s, char **endptr, int base);
unsigned long PCU_strtoul(const char *s, char **endptr, int base);
int PCU_atoi(const char *s);


#define ASSERT_STRTOL(s, base)	\
	do {\
		long ret1, ret2;\
		char *p1, *p2;\
		ret1 = strtol(s, &p1, base);\
		ret2 = PCU_strtol(s, &p2, base);\
		PCU_ASSERT_EQUAL(ret1, ret2);\
		PCU_ASSERT_PTR_EQUAL(p1, p2);\
	} while (0)

#define ASSERT_STRTOUL(s, base)	\
	do {\
		unsigned long ret1, ret2;\
		char *p1, *p2;\
		ret1 = strtoul(s, &p1, base);\
		ret2 = PCU_strtoul(s, &p2, base);\
		PCU_ASSERT_EQUAL(ret1, ret2);\
		PCU_ASSERT_PTR_EQUAL(p1, p2);\
	} while (0)

#define ASSERT_ATOI(s)	\
	do {\
		int ret1, ret2;\
		ret1 = atoi(s);\
		ret2 = PCU_atoi(s);\
		PCU_ASSERT_EQUAL(ret1, ret2);\
	} while (0)


void test_strtol(void)
{
	ASSERT_STRTOL("0", 10);
	ASSERT_STRTOL("0000", 10);
	ASSERT_STRTOL("123", 10);
	ASSERT_STRTOL("-0", 10);
	ASSERT_STRTOL("-123", 10);
	ASSERT_STRTOL("- 123", 10);
	ASSERT_STRTOL("	-123", 10);
	ASSERT_STRTOL("+0", 10);
	ASSERT_STRTOL("+123", 10);
	ASSERT_STRTOL("+ 123", 10);
	ASSERT_STRTOL("	+123", 10);
	ASSERT_STRTOL("  123aa", 10);
	ASSERT_STRTOL(" 	 123	 	", 10);
	ASSERT_STRTOL("zzz123zzz", 10);
	ASSERT_STRTOL("0x123", 10);
	ASSERT_STRTOL("zzz", 10);

	ASSERT_STRTOL("0", 16);
	ASSERT_STRTOL("0000", 16);
	ASSERT_STRTOL("0x0000", 16);
	ASSERT_STRTOL("123", 16);
	ASSERT_STRTOL("- 0123", 16);
	ASSERT_STRTOL("-0123", 16);
	ASSERT_STRTOL("-0", 16);
	ASSERT_STRTOL("-0x0", 16);
	ASSERT_STRTOL("+ 0123", 16);
	ASSERT_STRTOL("+0123", 16);
	ASSERT_STRTOL("+0", 16);
	ASSERT_STRTOL("+0x0", 16);
	ASSERT_STRTOL("0x123", 16);
	ASSERT_STRTOL("0x7eadBeef", 16);
	ASSERT_STRTOL("0X7EADBEEF", 16);
	ASSERT_STRTOL("	0012a", 16);
	ASSERT_STRTOL("  123aa", 16);
	ASSERT_STRTOL(" 	 123	 	", 16);
	ASSERT_STRTOL("zzz123zzz", 16);
	ASSERT_STRTOL("1zzz123zzz", 16);
	ASSERT_STRTOL("0z", 16);
/*    ASSERT_STRTOL("0x", 16);*/
/*    ASSERT_STRTOL("0xz", 16);*/
/*    ASSERT_STRTOL("0xZ123", 16);*/
/*    ASSERT_STRTOL("0xx123", 16);*/
	ASSERT_STRTOL("zzz", 16);

	ASSERT_STRTOL("0x123", 0);
	ASSERT_STRTOL("-0xABCD", 0);
	ASSERT_STRTOL("123", 0);
	ASSERT_STRTOL("-123", 0);
	ASSERT_STRTOL("+123", 0);
	ASSERT_STRTOL("1A3", 0);
	ASSERT_STRTOL("zzz", 0);
}

void test_strtoul(void)
{
	ASSERT_STRTOUL("0", 10);
	ASSERT_STRTOUL("0000", 10);
	ASSERT_STRTOUL("123", 10);
	ASSERT_STRTOUL("-0", 10);
	ASSERT_STRTOUL("-123", 10);
	ASSERT_STRTOUL("- 123", 10);
	ASSERT_STRTOUL("	-123", 10);
	ASSERT_STRTOUL("  123aa", 10);
	ASSERT_STRTOUL(" 	 123	 	", 10);
	ASSERT_STRTOUL("zzz123zzz", 10);
	ASSERT_STRTOUL("zzz", 10);

	ASSERT_STRTOUL("0", 16);
	ASSERT_STRTOUL("0000", 16);
	ASSERT_STRTOUL("0x0000", 16);
	ASSERT_STRTOUL("123", 16);
	ASSERT_STRTOUL("0123", 16);
	ASSERT_STRTOUL("-0", 16);
	ASSERT_STRTOUL("-0123", 16);
	ASSERT_STRTOUL("0x123", 16);
	ASSERT_STRTOUL("-0x123", 16);
	ASSERT_STRTOUL("-0x0", 16);
	ASSERT_STRTOUL("- 0x123", 16);
	ASSERT_STRTOUL("0xDeadBeef", 16);
	ASSERT_STRTOUL("0XDEADBEEF", 16);
	ASSERT_STRTOUL("-0XDEADBEEF", 16);
	ASSERT_STRTOUL("	0012a", 16);
	ASSERT_STRTOUL("  123aa", 16);
	ASSERT_STRTOUL(" 	 123	 	", 16);
	ASSERT_STRTOUL("zzz123zzz", 16);
	ASSERT_STRTOUL("1zzz123zzz", 16);
	ASSERT_STRTOUL("0z", 16);
/*    ASSERT_STRTOUL("0x", 16);*/
/*    ASSERT_STRTOUL("0xz", 16);*/
/*    ASSERT_STRTOUL("0xZ123", 16);*/
/*    ASSERT_STRTOUL("0xx123", 16);*/
	ASSERT_STRTOUL("zzz", 16);

	ASSERT_STRTOUL("0x123", 0);
	ASSERT_STRTOUL("-0xABCD", 0);
	ASSERT_STRTOUL("123", 0);
	ASSERT_STRTOUL("-123", 0);
	ASSERT_STRTOUL("+123", 0);
	ASSERT_STRTOUL("1A3", 0);
	ASSERT_STRTOUL("zzz", 0);
}

void test_atoi(void)
{
	ASSERT_ATOI("0");
	ASSERT_ATOI("0000");
	ASSERT_ATOI("123");
	ASSERT_ATOI("-0");
	ASSERT_ATOI("-123");
	ASSERT_ATOI("- 123");
	ASSERT_ATOI("	-123");
	ASSERT_ATOI("+0");
	ASSERT_ATOI("+123");
	ASSERT_ATOI("+ 123");
	ASSERT_ATOI("	+123");
	ASSERT_ATOI("  123aa");
	ASSERT_ATOI(" 	 123	 	");
	ASSERT_ATOI("zzz123zzz");
	ASSERT_ATOI("0x123");
	ASSERT_ATOI("zzz");
}


PCU_Suite *StrtolTest_suite(void)
{
	static PCU_Test tests[] = {
		{ "test_strtol", test_strtol },
		{ "test_strtoul", test_strtoul },
		{ "test_atoi", test_atoi },
	};
	static PCU_Suite suite = { "StrtolTest", tests, sizeof tests / sizeof tests[0] };
	return &suite;
}
