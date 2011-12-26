#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertPtrTest.c"

PCU_Suite *AssertPtrTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertPtrTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

