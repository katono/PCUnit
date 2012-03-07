#include "PCUnit/PCUnit.h"
#include <stdio.h>

PCU_Suite *AssertNotSupportedTest_suite(void);
PCU_Suite *AssertOutputTest_suite(void);

int main(void)
{
	const PCU_SuiteMethod suites[] = {
		AssertNotSupportedTest_suite,
		AssertOutputTest_suite,
	};
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_enable_color();
	PCU_run(suites, sizeof suites / sizeof suites[0]);
	return 0;
}

