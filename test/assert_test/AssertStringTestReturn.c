#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertStringTest.c"

PCU_Suite *AssertStringTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

