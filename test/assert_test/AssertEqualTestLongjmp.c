#include "../../PCUnit/PCUnit.h"

#include "AssertEqualTest.c"

PCU_Suite *AssertEqualTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertEqualTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

