#include <stdlib.h>
#include <stdio.h>
#include "point.h"

Point* point_alloc()
{
    Point* P = (Point*) malloc(sizeof(Point));
    mpz_init(P->x);
    mpz_init(P->y);

    return P;
}

int point_init(Point* P, const char* x, const char* y)
{
    if(P == NULL) {
        fprintf(stderr, "ERROR: point_create() -> invalid pointer\n");
        exit(1);
    }
    mpz_set_str(P->x, x, 10);
    mpz_set_str(P->y, y, 10);

    return 0;
}

int point_init_mpz_t(Point* P, const mpz_t x, const mpz_t y)
{
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: point_create_mpz_t() -> invalid pointer\n");
        return -1;
    }
    mpz_set(P->x, x);
    mpz_set(P->y, y);

    return 0;
}

int point_at_infinity(Point *P)
{
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: point_at_infinity() -> invalid pointer\n");
        return -1;
    }
    mpz_set_si(P->x, -1);
    mpz_set_si(P->y, -1);

    return 0;
}

int point_is_equal(const Point* P, const Point* Q)
{
    int is_equal = 0;
    if(mpz_cmp(P->x, Q->x) == 0 && mpz_cmp(P->y, Q->y) == 0)
        is_equal = 1;

    return is_equal;
}

int point_copy(Point* self, const Point* other)
{
    if(self == NULL) {
        fprintf(stderr, "ERROR: point_copy() -> invalid pointer\n");
        return -1;
    }
    mpz_set(self->x, other->x);
    mpz_set(self->y, other->y);
    
    return 0;
}

void point_destroy(Point* P)
{
    mpz_clear(P->x);
    mpz_clear(P->y);
    free(P);
}
