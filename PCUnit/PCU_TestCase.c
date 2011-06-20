#include "PCU_TestCase.h"
#include "PCU_Test.h"
#include "PCU_Libc.h"

static PCU_TestCase *current_case;

static void PCU_TestCase_clear_result(PCU_TestCase *self)
{
	self->initialize_error = 0;
	self->cleanup_error = 0;
	PCU_MEMSET(&self->result, 0, sizeof(self->result));
}

void PCU_TestCase_reset(PCU_TestCase *self)
{
	int i;
	PCU_Test *p;
	for (i = 0, p = self->tests; p->name != 0; i++, p++) {
		PCU_Test_reset(p);
	}
	PCU_TestCase_clear_result(self);
	self->result.num_tests = i;
}

static int PCU_TestCase_initialize(const PCU_TestCase *self)
{
	int ret = 0;
	if (self->initialize) {
		ret = self->initialize();
	}
	return ret;
}

static int PCU_TestCase_cleanup(const PCU_TestCase *self)
{
	int ret = 0;
	if (self->cleanup) {
		ret = self->cleanup();
	}
	return ret;
}

void PCU_TestCase_run(PCU_TestCase *self)
{
	PCU_Test *p;
	current_case = self;

	self->initialize_error = PCU_TestCase_initialize(self);
	if (self->initialize_error) {
		self->result.num_errors_initialize++;
		return;
	}
	for (p = self->tests; p->name != 0; p++) {
		PCU_TestResult test_result;
		PCU_Test_run(p);
		PCU_Test_get_result(p, &test_result);
		self->result.test_result.num_asserts         += test_result.num_asserts;
		self->result.test_result.num_asserts_ran     += test_result.num_asserts_ran;
		self->result.test_result.num_asserts_failed  += test_result.num_asserts_failed;
		self->result.test_result.num_errors_setup    += test_result.num_errors_setup;
		self->result.test_result.num_errors_teardown += test_result.num_errors_teardown;
		self->result.num_tests_ran++;
		if (test_result.num_asserts_failed > 0 || test_result.num_errors_setup > 0 || test_result.num_errors_teardown > 0) {
			self->result.num_tests_failed++;
		}
	}
	self->cleanup_error = PCU_TestCase_cleanup(self);
	if (self->cleanup_error) {
		self->result.num_errors_cleanup++;
		return;
	}
}

void PCU_TestCase_run_selected(PCU_TestCase *self, int idx)
{
	PCU_TestResult test_result;
	current_case = self;

	self->initialize_error = PCU_TestCase_initialize(self);
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
	self->result.num_tests_ran++;
	if (test_result.num_asserts_failed > 0 || test_result.num_errors_setup > 0 || test_result.num_errors_teardown > 0) {
		self->result.num_tests_failed++;
	}
	self->cleanup_error = PCU_TestCase_cleanup(self);
	if (self->cleanup_error) {
		self->result.num_errors_cleanup++;
		return;
	}
}

void PCU_TestCase_get_result(PCU_TestCase *self, PCU_TestCaseResult *result)
{
	*result = self->result;
}

const char *PCU_case_name(void)
{
	return current_case->name;
}

