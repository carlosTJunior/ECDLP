#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "unittests.h"
#include "../ecc.h"

/* global vars */
EllipticCurve ec;
Point P, Q, Inf;

void testEccCreate() {
    printf("testEccCreate ");

    EllipticCurve ec;
    ec = EllipticCurve(23, 1, 1, 28);
    
    assert(ec.p == 23);
    assert(ec.a == 1);
    assert(ec.b == 1);
    assert(ec.order == 28);
}

void testEccAdd1() {
    printf("testEccAdd1 ");

    Point R;

    ecc_add(R, ec, P, Q);

    assert(R.x == 17);
    assert(R.y == 20);
}

void testEccAdd2() {
    printf("testEccAdd2 ");

    Point R;

    ecc_add(R, ec, P, Inf);

    assert(R.x == P.x);
    assert(R.y == P.y);
}

void testEccAdd3() {
    printf("testEccAdd3 ");

    Point R;

    ecc_add(R, ec, Inf, Q);

    assert(R.x == Q.x);
    assert(R.y == Q.y);
}

/* This test will have buffer overflow for type long long */
void testEccAdd4() {
    printf("testEccAdd4 ");

    EllipticCurve ec1;
    ec1 = EllipticCurve(2879867477, 62293, 47905, 2879882063);
    Point R;

    Point tempQ(2023576232, 137974030);
    ecc_add(R, ec1, tempQ, tempQ);

    assert(R.x == 991038922);
    assert(R.y == 753333067);
}

void testEccMul1() {
    printf("testEccMul1 ");

    Point R;

    ecc_mul(R, ec, 2, P);

    assert(R.x == 7);
    assert(R.y == 12);
}

void testEccMul2() {
    printf("testEccMul2 ");

    Point R;

    ecc_mul(R, ec, 3, P);

    assert(R.x == 19);
    assert(R.y == 5);
}

void testEccMul3() {
    printf("testEccMul3 ");

    Point R;

    ecc_mul(R, ec, 7, P);

    assert(R.x == 11);
    assert(R.y == 3);
}

void init_vars() {
    ec = EllipticCurve(23, 1, 1, 28);
    P = Point(3, 10);
    Q = Point(9, 7);
    point_at_infinity(Inf);
    //Inf = Point(-1, -1);
}

test_func ecc_functions[] = { 
    testEccCreate,
    testEccAdd1,
    testEccAdd2,
    testEccAdd3,
    testEccAdd4, //Buffer overflow for long long
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
