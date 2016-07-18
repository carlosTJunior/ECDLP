#ifndef _POINT
#define _POINT

#include <gmp.h>

typedef struct _point {
    mpz_t x;
    mpz_t y;
} Point;

extern Point point_create(char*, char*);
extern Point point_create_mpz_t(mpz_t, mpz_t);

#endif
