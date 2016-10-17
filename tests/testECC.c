#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "unittests.h"
#include "../ecc.h"

/* global vars */
EllipticCurve ec;
Point *P, *Q;

void testEccCreate() {
    printf("testEccCreate ");

    EllipticCurve ec;
    ec = ecc_create(23, 1, 1, 28);
    
    assert(ec.p == 23);
    assert(ec.a == 1);
    assert(ec.b == 1);
    assert(ec.order == 28);
}

void testEccAdd() {
    printf("testEccAdd ");

    Point *R;
    R = point_alloc();

    ecc_add(R, ec, P, Q);

    assert(R->x == 17);
    assert(R->y == 20);

    point_destroy(R);
}

void testEccMul1() {
    printf("testEccMul1 ");

    Point *R;
    R = point_alloc();

    ecc_mul(R, ec, 2, P);

    assert(R->x == 7);
    assert(R->y == 12);

    point_destroy(R);
}

void testEccMul2() {
    printf("testEccMul2 ");

    Point *R;
    R = point_alloc();

    ecc_mul(R, ec, 3, P);

    assert(R->x == 19);
    assert(R->y == 5);

    point_destroy(R);
}

void testEccMul3() {
    printf("testEccMul3 ");

    Point *R;
    R = point_alloc();

    ecc_mul(R, ec, 7, P);

    assert(R->x == 11);
    assert(R->y == 3);

    point_destroy(R);
}

void init_vars() {
    ec = ecc_create(23, 1, 1, 28);
    P = point_alloc();
    Q = point_alloc();
    point_init(P, 3, 10);
    point_init(Q, 9, 7);
}

test_func ecc_functions[] = { 
    testEccCreate,
    testEccAdd,
    testEccMul1,
    testEccMul2,
    testEccMul3,
    NULL 
};

void run_ecc_tests() {
    int i = 0;

    init_vars();
    while(ecc_functions[i] != NULL) {
        (*ecc_functions[i])();
        printf("OK\n");
        i++;
        numTests++;
    }
}
