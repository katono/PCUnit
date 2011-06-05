#include "../PCUnit/include/PCUnit.h"
#include <string.h>
#include <stdio.h>

extern PCU_Test string_tests[];
extern PCU_Test sprintf_tests[];
extern PCU_Test strtol_tests[];
extern PCU_Test assert_tests[];

PCU_TestCase suite[] = {
	{ "sprintf_tests", sprintf_tests },
	{ "string_tests", string_tests },
	{ "strtol_tests", strtol_tests },
	PCU_NULL,
};

PCU_TestCase suite_assert[] = {
	{ "assert_tests", assert_tests },
	PCU_NULL,
};

int main(int argc, char **argv)
{
	PCU_enable_color();
	PCU_set_getchar(getchar);
	if (argc > 1) {
		if (strcmp(argv[1], "assert") == 0) {
			PCU_run_interactive(suite_assert);
		} else {
			PCU_run_interactive(suite);
		}
	} else {
		PCU_run(suite);
	}
	return 0;
}

