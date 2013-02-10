#include "PCUnit/PCUnit.h"
#include <stdio.h>


PCU_Suite *RingBufTest_suite(void);
PCU_Suite *StrTokenTest_suite(void);

int main(int argc, char **argv)
{
	const PCU_SuiteMethod suites[] = {
		RingBufTest_suite,
		StrTokenTest_suite,
	};

	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_enable_color();
/*    PCU_disable_color();*/
	if (argc >= 2) {
		PCU_console_run(suites, sizeof suites / sizeof suites[0]);
	} else {
		return PCU_run(suites, sizeof suites / sizeof suites[0]);
	}
	return 0;
}

