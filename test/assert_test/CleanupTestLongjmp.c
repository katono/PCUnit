#include "../../PCUnit/PCUnit.h"

#include "CleanupTest.c"

PCU_Suite *CleanupTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"CleanupTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

