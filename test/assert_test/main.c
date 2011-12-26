#include "../../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>


PCU_Suite *AssertDoubleTestLongjmp_suite(void);
PCU_Suite *AssertDoubleTestReturn_suite(void);
PCU_Suite *AssertEqualTestLongjmp_suite(void);
PCU_Suite *AssertEqualTestReturn_suite(void);
PCU_Suite *AssertOperatorTestLongjmp_suite(void);
PCU_Suite *AssertOperatorTestReturn_suite(void);
PCU_Suite *AssertPtrTestLongjmp_suite(void);
PCU_Suite *AssertPtrTestReturn_suite(void);
PCU_Suite *AssertStringTestLongjmp_suite(void);
PCU_Suite *AssertStringTestReturn_suite(void);
PCU_Suite *AssertStringWTestLongjmp_suite(void);
PCU_Suite *AssertStringWTestReturn_suite(void);
PCU_Suite *AssertTestLongjmp_suite(void);
PCU_Suite *AssertTestReturn_suite(void);
PCU_Suite *CleanupTestLongjmp_suite(void);
PCU_Suite *CleanupTestReturn_suite(void);
PCU_Suite *FailTestLongjmp_suite(void);
PCU_Suite *FailTestReturn_suite(void);
PCU_Suite *InitializeTestLongjmp_suite(void);
PCU_Suite *InitializeTestReturn_suite(void);
PCU_Suite *SetupTestLongjmp_suite(void);
PCU_Suite *SetupTestReturn_suite(void);

int main(int argc, char **argv)
{
	const PCU_SuiteMethod suites[] = {
		AssertDoubleTestLongjmp_suite,
		AssertDoubleTestReturn_suite,
		AssertEqualTestLongjmp_suite,
		AssertEqualTestReturn_suite,
		AssertOperatorTestLongjmp_suite,
		AssertOperatorTestReturn_suite,
		AssertPtrTestLongjmp_suite,
		AssertPtrTestReturn_suite,
		AssertStringTestLongjmp_suite,
		AssertStringTestReturn_suite,
		AssertStringWTestLongjmp_suite,
		AssertStringWTestReturn_suite,
		AssertTestLongjmp_suite,
		AssertTestReturn_suite,
		CleanupTestLongjmp_suite,
		CleanupTestReturn_suite,
		FailTestLongjmp_suite,
		FailTestReturn_suite,
		InitializeTestLongjmp_suite,
		InitializeTestReturn_suite,
		SetupTestLongjmp_suite,
		SetupTestReturn_suite,
	};

	PCU_enable_color();
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	if (argc > 1) {
		PCU_console_run(suites, sizeof suites / sizeof *suites);
	} else {
		PCU_run(suites, sizeof suites / sizeof *suites);
	}
	return 0;
}

