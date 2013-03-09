#include "../../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>


PCU_Suite *FormatNoStdargTest_suite(void);
PCU_Suite *StringTest_suite(void);
PCU_Suite *StrtolTest_suite(void);

int main(void)
{
	const PCU_SuiteMethod suites[] = {
		FormatNoStdargTest_suite,
		StringTest_suite,
		StrtolTest_suite,
	};

	PCU_enable_color();
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_set_verbose(1);
	return PCU_run(suites, sizeof suites / sizeof *suites);
}

