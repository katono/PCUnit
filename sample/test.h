#ifndef TEST_H
#define TEST_H

#include "PCUnit.h"

int RingBuf_tests_initialize(void);
int RingBuf_tests_cleanup(void);
int RingBuf_tests_setup(void);
int RingBuf_tests_teardown(void);
int StrToken_tests_setup(void);
int StrToken_tests_teardown(void);
extern PCU_Test RingBuf_tests[];
extern PCU_Test StrToken_tests[];

#endif /* TEST_H */
