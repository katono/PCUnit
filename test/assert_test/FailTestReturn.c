#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "FailTest.c"

PCU_Suite *FailTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"FailTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

