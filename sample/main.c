#include "PCUnit.h"
#include <stdio.h>


PCU_Suite *RingBufTest_suite(void);
PCU_Suite *StrTokenTest_suite(void);

int main(int argc, char **argv)
{
	PCU_SuiteMethod suites[] = {
		RingBufTest_suite,
		StrTokenTest_suite,
	};

	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_enable_color();
/*    PCU_disable_color();*/
	if (argc >= 2) {
		PCU_set_mode(PCU_MODE_INTERACTIVE);
	}

	PCU_run(suites, sizeof suites / sizeof suites[0]);
	return 0;
}

