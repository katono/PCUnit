#ifndef PCU_TEST_H_INCLUDED
#define PCU_TEST_H_INCLUDED

#include "PCUnit.h"

void PCU_Test_reset(PCU_Test *self);
void PCU_Test_run(PCU_Test *self);
int PCU_Test_is_skipped(const PCU_Test *self);
int PCU_Test_is_failed(const PCU_Test *self);


#endif /* PCU_TEST_H_INCLUDED */
