#include "../../PCUnit/PCUnit.h"

#include "AssertStringWTest.c"

PCU_Suite *AssertStringWTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringWTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

