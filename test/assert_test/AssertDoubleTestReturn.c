#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertDoubleTest.c"

PCU_Suite *AssertDoubleTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertDoubleTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

