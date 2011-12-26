#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "CleanupTest.c"

PCU_Suite *CleanupTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"CleanupTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

