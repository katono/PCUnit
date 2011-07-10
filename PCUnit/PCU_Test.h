#ifndef PCU_TEST_H_INCLUDED
#define PCU_TEST_H_INCLUDED

#include "PCUnit.h"

void PCU_Test_reset(PCU_Test *self, const PCU_Suite *suite);
void PCU_Test_run(PCU_Test *self);
void PCU_Test_get_result(PCU_Test *self, PCU_TestResult *result);
int PCU_Test_is_repeated_test(PCU_Test *self);
int PCU_Test_is_skipped(PCU_Test *self);
int PCU_Test_is_failed(PCU_Test *self);


#endif /* PCU_TEST_H_INCLUDED */
