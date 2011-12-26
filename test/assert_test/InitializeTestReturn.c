#define PCU_NO_SETJMP
#include "../../PCUnit/PCUnit.h"

#include "InitializeTest.c"

PCU_Suite *InitializeTestReturn_suite(void)
{
	static PCU_Suite suite = {
		"InitializeTestReturn", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

