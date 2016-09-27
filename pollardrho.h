#ifndef _POLLARD_RHO
#define _POLLARD_RHO

#include "ecc.h"

typedef struct _triple {
    Point* point;
    mpz_t c;
    mpz_t d;
} Triple;

extern gmp_randstate_t state;

extern int pollardrho_serial(mpz_t, const EllipticCurve, const Point*, \
       const Point*);

#endif
