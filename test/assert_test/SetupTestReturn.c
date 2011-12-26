#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "SetupTest.c"

PCU_Suite *SetupTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"SetupTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

