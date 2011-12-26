#include "../../PCUnit/PCUnit.h"

#include "AssertDoubleTest.c"

PCU_Suite *AssertDoubleTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertDoubleTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

