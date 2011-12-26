#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertStringWTest.c"

PCU_Suite *AssertStringWTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringWTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

