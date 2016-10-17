#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "unittests.h"
#include "../point.h"


void testCreateDestroyPoint() {
    printf("testCreateDestroyPoint ");
    Point* P;
    P = point_alloc();
    assert(P != NULL);

    point_destroy(P);
}

void testInitPoint() {
    printf("testInitPoint ");
    Point* P;
    P = point_alloc();

    point_init(P, 7, 23);
    assert(P->x == 7);
    assert(P->y == 23);

    point_destroy(P);
}

void testPointAtInfinity() {
    printf("testPointAtInfinity ");
    Point* P;
    P = point_alloc();
    point_at_infinity(P);

    assert(P->x == -1);
    assert(P->y == -1);

    point_destroy(P);
}

void testPointIsEqual() {
    printf("testPointIsEqual ");
    Point *P, *Q;
    P = point_alloc();
    Q = point_alloc();

    point_init(P, 5, 9);
    point_init(Q, 5, 9);

    assert(point_is_equal(P, Q));

    point_destroy(P);
    point_destroy(Q);
}

void testPointCopy() {
    printf("testPointCopy ");
    Point *P, *Q;
    P = point_alloc();
    Q = point_alloc();

    point_init(P, 5, 9);
    point_copy(Q, P);

    assert(P->x == Q->x);
    assert(P->y == Q->y);

    point_destroy(P);
    point_destroy(Q);
}

test_func point_functions[] = { 
    testCreateDestroyPoint, 
    testInitPoint, 
    testPointAtInfinity, 
    testPointIsEqual,
    testPointCopy,
    NULL 
};

void run_point_tests() {
    int i = 0;

    while(point_functions[i] != NULL) {
        (*point_functions[i])();
        printf("OK\n");
        i++;
        numTests++;
    }
}
