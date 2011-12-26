#include "../../PCUnit/PCUnit.h"

#include "AssertPtrTest.c"

PCU_Suite *AssertPtrTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertPtrTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

