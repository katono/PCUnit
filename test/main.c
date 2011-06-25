#include "../PCUnit/PCUnit.h"
#include <string.h>
#include <stdio.h>

extern PCU_Test string_tests[];
extern PCU_Test snprintf_tests[];
extern PCU_Test strtol_tests[];
extern PCU_Test assert_tests[];

PCU_Suite suites[] = {
	{ "snprintf_tests", snprintf_tests },
	{ "string_tests", string_tests },
	{ "strtol_tests", strtol_tests },
	PCU_NULL,
};

int assert_tests_setup(void);
int assert_tests_teardown(void);

PCU_Suite suites_assert[] = {
	{ "assert_tests", assert_tests, assert_tests_setup, assert_tests_teardown },
	PCU_NULL,
};

int main(int argc, char **argv)
{
	PCU_enable_color();
	PCU_set_getchar(getchar);
	if (argc > 1) {
		PCU_set_mode(PCU_MODE_INTERACTIVE);
		if (strcmp(argv[1], "assert") == 0) {
			PCU_run(suites_assert);
		} else {
			PCU_run(suites);
		}
	} else {
		PCU_run(suites);
	}
	return 0;
}

