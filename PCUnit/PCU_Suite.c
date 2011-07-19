#include "PCU_Suite.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_Suite *current_suite;
static PCU_TestResult test_result;

static void PCU_Suite_clear_result(PCU_Suite *self)
{
	self->initialize_error = 0;
	self->cleanup_error = 0;
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_Suite_reset(PCU_Suite *self)
{
	int i;
	PCU_Test *p;
	for (i = 0, p = self->tests; i < self->ntests; i++, p++) {
		PCU_Test_reset(p, self);
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

void PCU_Suite_run(PCU_Suite *self)
{
	int i;
	PCU_Test *p;
	current_suite = self;

	self->initialize_error = PCU_Suite_initialize(self);
	if (self->initialize_error) {
		self->result.num_errors_initialize++;
		return;
	}
	for (i = 0, p = self->tests; i < self->ntests; i++, p++) {
		PCU_Test_run(p);
		PCU_Test_get_result(p, &test_result);
		self->result.test_result.num_asserts         += test_result.num_asserts;
		self->result.test_result.num_asserts_ran     += test_result.num_asserts_ran;
		self->result.test_result.num_asserts_failed  += test_result.num_asserts_failed;
		self->result.test_result.num_errors_setup    += test_result.num_errors_setup;
		self->result.test_result.num_errors_teardown += test_result.num_errors_teardown;
		if (!PCU_Test_is_skipped(p)) {
			self->result.num_tests_ran++;
		}
		if (test_result.num_asserts_failed > 0 || test_result.num_errors_setup > 0 || test_result.num_errors_teardown > 0) {
			self->result.num_tests_failed++;
		}
	}
	self->cleanup_error = PCU_Suite_cleanup(self);
	if (self->cleanup_error) {
		self->result.num_errors_cleanup++;
		return;
	}
}

void PCU_Suite_run_selected(PCU_Suite *self, int idx)
{
	current_suite = self;

	self->initialize_error = PCU_Suite_initialize(self);
	if (self->initialize_error) {
		self->result.num_errors_initialize++;
		return;
	}
	PCU_Test_run(&self->tests[idx]);
	PCU_Test_get_result(&self->tests[idx], &test_result);
	self->result.test_result.num_asserts         += test_result.num_asserts;
	self->result.test_result.num_asserts_ran     += test_result.num_asserts_ran;
	self->result.test_result.num_asserts_failed  += test_result.num_asserts_failed;
	self->result.test_result.num_errors_setup    += test_result.num_errors_setup;
	self->result.test_result.num_errors_teardown += test_result.num_errors_teardown;
	if (!PCU_Test_is_skipped(&self->tests[idx])) {
		self->result.num_tests_ran++;
	}
	if (test_result.num_asserts_failed > 0 || test_result.num_errors_setup > 0 || test_result.num_errors_teardown > 0) {
		self->result.num_tests_failed++;
	}
	self->cleanup_error = PCU_Suite_cleanup(self);
	if (self->cleanup_error) {
		self->result.num_errors_cleanup++;
		return;
	}
}

void PCU_Suite_get_result(PCU_Suite *self, PCU_SuiteResult *result)
{
	PCU_MEMCPY(result, &self->result, sizeof *result);
}

const char *PCU_suite_name(void)
{
	return current_suite->name;
}

