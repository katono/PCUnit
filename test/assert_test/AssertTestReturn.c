#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertTest.c"


PCU_Suite *AssertTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

