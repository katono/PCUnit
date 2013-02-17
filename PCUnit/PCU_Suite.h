#ifndef PCU_SUITE_H_INCLUDED
#define PCU_SUITE_H_INCLUDED

#include "PCUnit.h"

void PCU_Suite_reset(PCU_Suite *self);
void PCU_Suite_run(PCU_Suite *self);
void PCU_Suite_run_selected(PCU_Suite *self, int idx);
const PCU_SuiteResult *PCU_Suite_get_result(PCU_Suite *self);


#endif /* PCU_SUITE_H_INCLUDED */
