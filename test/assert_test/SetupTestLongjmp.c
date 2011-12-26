#include "../../PCUnit/PCUnit.h"

#include "SetupTest.c"

PCU_Suite *SetupTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"SetupTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

