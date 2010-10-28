#ifndef TEST_H
#define TEST_H

#include "PCUnit.h"

int RingBuf_tests_initialize(void);
int RingBuf_tests_cleanup(void);
extern PCU_Test RingBuf_tests[];
extern PCU_Test StrToken_tests[];

#endif /* TEST_H */
