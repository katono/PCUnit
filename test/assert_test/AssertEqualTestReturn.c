#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertEqualTest.c"

PCU_Suite *AssertEqualTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertEqualTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

