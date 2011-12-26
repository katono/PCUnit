#include "../../PCUnit/PCUnit.h"

#include "AssertOperatorTest.c"

PCU_Suite *AssertOperatorTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertOperatorTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

