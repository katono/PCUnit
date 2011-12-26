#include "../../PCUnit/PCUnit.h"

#include "InitializeTest.c"

PCU_Suite *InitializeTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"InitializeTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

