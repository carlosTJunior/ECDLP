#ifndef _POLLARD_RHO
#define _POLLARD_RHO

#include "ecc.h"

/* number of branches */
#define L 32

typedef struct _triple {
    Point point;
    mpz_t c;
    mpz_t d;
} Triple;

/* to generate random numbers, it's necessary to set a state */
extern gmp_randstate_t state;

/* General Pollard Rho functions */
extern int init_branches(Triple *branches, const EllipticCurve ec, const Point* P,\
       const Point* Q);
extern unsigned long int partition_function(const Point* P);
extern void random_number(mpz_t result, const mpz_t max);
extern int calculate_result(mpz_t result, const mpz_t c1, const mpz_t c2, \
        const mpz_t d1, const mpz_t d2, const mpz_t order);

/* Pollard Rho algorithms */
extern int pollardrho_serial(mpz_t, const EllipticCurve, const Point*, \
       const Point*);

#endif
