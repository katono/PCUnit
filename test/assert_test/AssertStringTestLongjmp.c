#include "../../PCUnit/PCUnit.h"

#include "AssertStringTest.c"

PCU_Suite *AssertStringTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertStringTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

