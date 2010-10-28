#ifndef PCU_TESTCASE_H_INCLUDED
#define PCU_TESTCASE_H_INCLUDED

#include "PCUnit.h"

void PCU_TestCase_reset(PCU_TestCase *self);
void PCU_TestCase_run(PCU_TestCase *self);
void PCU_TestCase_run_selected(PCU_TestCase *self, int idx);
void PCU_TestCase_get_result(PCU_TestCase *self, PCU_TestCaseResult *result);


#endif /* PCU_TESTCASE_H_INCLUDED */
