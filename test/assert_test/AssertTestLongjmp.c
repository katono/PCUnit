#include "../../PCUnit/PCUnit.h"

#include "AssertTest.c"


PCU_Suite *AssertTestLongjmp_suite(void)
{
	static PCU_Suite suite = {
		"AssertTestLongjmp", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}
