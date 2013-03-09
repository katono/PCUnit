#include "../../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>


PCU_Suite *FormatTest_suite(void);
PCU_Suite *FormatWTest_suite(void);

int main(void)
{
	const PCU_SuiteMethod suites[] = {
		FormatTest_suite,
		FormatWTest_suite,
	};

	PCU_enable_color();
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_set_verbose(1);
	return PCU_run(suites, sizeof suites / sizeof *suites);
}

