#include "../../PCUnit/PCUnit.h"

#include "FailTest.c"

PCU_Suite *FailTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"FailTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

