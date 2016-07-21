#include <stdlib.h>
#include <stdio.h>
#include "point.h"

Point* point_create(const char* x, const char* y)
{
    Point *P = (Point*) malloc(sizeof(Point));
    if(P == NULL) {
        fprintf(stderr, "ERROR: point_create() -> Cannot create pointer\n");
        exit(1);
    }
    mpz_init_set_str(P->x, x, 10);
    mpz_init_set_str(P->y, y, 10);

    return P;
}

Point* point_create_mpz_t(const mpz_t x, const mpz_t y)
{
    Point* P = (Point*) malloc(sizeof(Point));
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: point_create_mpz_t() -> Cannot create pointer\n");
        exit(1);
    }
    mpz_init_set(P->x, x);
    mpz_init_set(P->y, y);

    return P;
}

Point* point_at_infinity()
{
    Point* P = (Point*) malloc(sizeof(Point));
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: point_at_infinity() -> Cannot create pointer\n");
        exit(1);
    }
    mpz_init_set_si(P->x, -1);
    mpz_init_set_si(P->y, -1);

    return P;
}

int point_is_equal(const Point* P, const Point* Q)
{
    int is_equal = 0;
    if(mpz_cmp(P->x, Q->x) == 0 && mpz_cmp(P->y, Q->y) == 0)
        is_equal = 1;

    return is_equal;
}

Point* point_copy(const Point* P)
{
    Point* C = (Point*) malloc(sizeof(Point));
    if(C == NULL) {
        fprintf(stderr, "ERROR: point_copy() -> Cannot create pointer\n");
        exit(1);
    }
    mpz_init_set(C->x, P->x);
    mpz_init_set(C->y, P->y);
    
    return C;
}

void point_destroy(Point* P)
{
    mpz_clear(P->x);
    mpz_clear(P->y);
    free(P);
}
