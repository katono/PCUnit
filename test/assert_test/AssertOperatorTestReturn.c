#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "AssertOperatorTest.c"

PCU_Suite *AssertOperatorTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"AssertOperatorTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

