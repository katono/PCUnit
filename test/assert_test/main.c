#include "../../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>

PCU_Suite *AssertDoubleTest_suite(void);
PCU_Suite *AssertEqualTest_suite(void);
PCU_Suite *AssertMemoryTest_suite(void);
PCU_Suite *AssertOperatorTest_suite(void);
PCU_Suite *AssertPtrTest_suite(void);
PCU_Suite *AssertStringTest_suite(void);
PCU_Suite *AssertStringWTest_suite(void);
PCU_Suite *AssertTest_suite(void);
PCU_Suite *CleanupTest_suite(void);
PCU_Suite *FailTest_suite(void);
PCU_Suite *InitializeTest_suite(void);
PCU_Suite *SetupTest_suite(void);

int main(int argc, char **argv)
{
	const PCU_SuiteMethod suites[] = {
		AssertDoubleTest_suite,
		AssertEqualTest_suite,
		AssertMemoryTest_suite,
		AssertOperatorTest_suite,
		AssertPtrTest_suite,
		AssertStringTest_suite,
		AssertStringWTest_suite,
		AssertTest_suite,
		CleanupTest_suite,
		FailTest_suite,
		InitializeTest_suite,
		SetupTest_suite,
	};

	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	if (argc > 1 && !strcmp(argv[1], "console")) {
		PCU_enable_color();
		PCU_console_run(suites, sizeof suites / sizeof *suites);
	} else if (argc > 1 && !strcmp(argv[1], "nocolor")) {
		PCU_disable_color();
		if (argc > 2 && !strcmp(argv[2], "verbose")) {
			PCU_set_verbose(1);
		}
		return PCU_run(suites, sizeof suites / sizeof *suites);
	} else {
		PCU_enable_color();
		return PCU_run(suites, sizeof suites / sizeof *suites);
	}
	return 0;
}

