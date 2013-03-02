#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_Suite *current_suite;

static void PCU_Suite_clear_result(PCU_Suite *self)
{
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_Suite_reset(PCU_Suite *self)
{
	int i;
	PCU_Test *p;
	for (i = 0, p = self->tests; i < self->ntests; i++, p++) {
		PCU_Test_reset(p);
	}
	PCU_Suite_clear_result(self);
	self->result.num_tests = i;
}

static int PCU_Suite_initialize(const PCU_Suite *self)
{
	int ret = 0;
	if (self->initialize) {
		ret = self->initialize();
	}
	return ret;
}

static int PCU_Suite_cleanup(const PCU_Suite *self)
{
	int ret = 0;
	if (self->cleanup) {
		ret = self->cleanup();
	}
	return ret;
}

int PCU_Suite_setup(void)
{
	int ret = 0;
	if (current_suite->setup) {
		ret = current_suite->setup();
	}
	return ret;
}

int PCU_Suite_teardown(void)
{
	int ret = 0;
	if (current_suite->teardown) {
		ret = current_suite->teardown();
	}
	return ret;
}

void PCU_Suite_run(PCU_Suite *self)
{
	int i;
	PCU_Test *p;
	current_suite = self;

	self->result.initialize_error = PCU_Suite_initialize(self);
	if (self->result.initialize_error) {
		return;
	}
	for (i = 0, p = self->tests; i < self->ntests; i++, p++) {
		PCU_Test_run(p);
		if (!PCU_Test_is_skipped(p)) {
			self->result.num_tests_ran++;
		}
		if (PCU_Test_is_failed(p)) {
			self->result.num_tests_failed++;
		}
	}
	self->result.cleanup_error = PCU_Suite_cleanup(self);
}

#ifndef PCU_NO_CONSOLE_RUN
void PCU_Suite_run_selected(PCU_Suite *self, int idx)
{
	PCU_Test *p = self->tests + idx;
	current_suite = self;

	self->result.initialize_error = PCU_Suite_initialize(self);
	if (self->result.initialize_error) {
		return;
	}
	PCU_Test_run(p);
	if (!PCU_Test_is_skipped(p)) {
		self->result.num_tests_ran++;
	}
	if (PCU_Test_is_failed(p)) {
		self->result.num_tests_failed++;
	}
	self->result.cleanup_error = PCU_Suite_cleanup(self);
}
#endif

const char *PCU_suite_name(void)
{
	return current_suite->name;
}

