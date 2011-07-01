#include "../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>


PCU_Suite *AssertTest_suite(void);
PCU_Suite *SnprintfTest_suite(void);
PCU_Suite *StringTest_suite(void);
PCU_Suite *StrtolTest_suite(void);

int main(int argc, char **argv)
{
	const PCU_SuiteMethod assert_suites[] = {
		AssertTest_suite,
	};
	const PCU_SuiteMethod suites[] = {
		SnprintfTest_suite,
		StringTest_suite,
		StrtolTest_suite,
	};

	PCU_enable_color();
	PCU_set_getchar(getchar);
	if (argc > 1) {
		PCU_set_mode(PCU_MODE_INTERACTIVE);
		if (strcmp(argv[1], "assert") == 0) {
			PCU_run(assert_suites, sizeof assert_suites / sizeof *assert_suites);
		} else {
			PCU_run(suites, sizeof suites / sizeof *suites);
		}
	} else {
		PCU_run(suites, sizeof suites / sizeof *suites);
	}
	return 0;
}

