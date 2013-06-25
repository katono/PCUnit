#include "PCUnit/PCUnit.h"
#include <stdio.h>

PCU_Suite *RingBufTest_suite(void);
PCU_Suite *StrTokenTest_suite(void);

int main(int argc, char *argv[])
{
	const PCU_SuiteMethod suites[] = {
		RingBufTest_suite,
		StrTokenTest_suite,
	};
	int i;
	int console_mode = 0;
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_enable_color();
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'c':
				console_mode = 1;
				break;
			case 'v':
				PCU_set_verbose(1);
				break;
			case 'd':
				PCU_disable_color();
				break;
			default:
				break;
			}
		}
	}
	if (console_mode) {
		return PCU_console_run(suites, sizeof suites / sizeof suites[0]);
	}
	return PCU_run(suites, sizeof suites / sizeof suites[0]);
}

