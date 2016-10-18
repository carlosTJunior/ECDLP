#ifndef _UNIT_TESTS
#define _UNIT_TESTS

#define TRUE 1
#define FALSE 0

typedef void (*test_func)(void);

/* global var: number of tests executed */
int numTests;

extern void run_point_tests();
extern void run_ecc_tests();
extern void run_pollardrho_tests();

#endif
