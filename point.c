#include <stdlib.h>
#include <stdio.h>
#include "point.h"

Point* point_alloc()
{
    Point* P = (Point*) malloc(sizeof(Point));
    //mpz_init(P->x);
    //mpz_init(P->y);

    return P;
}

int point_init(Point* P, const BigInt x, const BigInt y)
{
    if(P == NULL) {
        fprintf(stderr, "ERROR: point_create() -> invalid pointer\n");
        exit(1);
    }

    P->x = x;
    P->y = y;
    return 0;
}

int point_at_infinity(Point *P)
{
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: point_at_infinity() -> invalid pointer\n");
        return -1;
    }
    P->x = -1;
    P->y = -1;

    return 0;
}

int point_is_equal(const Point* P, const Point* Q)
{
    int is_equal = 0;
    if((P->x == Q->x) && (P->y == Q->y))
        is_equal = 1;

    return is_equal;
}

int point_copy(Point* self, const Point* other)
{
    if(self == NULL) {
        fprintf(stderr, "ERROR: point_copy() -> invalid pointer\n");
        return -1;
    }
    self->x = other->x;
    self->y = other->y;
    
    return 0;
}

void point_destroy(Point* P)
{
    //mpz_clear(P->x);
    //mpz_clear(P->y);
    free(P);
}
