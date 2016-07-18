#ifndef _ECC
#define _ECC

/*#define DEBUG*/

#include "point.h"

typedef struct _ecc {
    mpz_t p;
    mpz_t a;
    mpz_t b;
    mpz_t order;
} EllipticCurve;

extern EllipticCurve ecc_create(char*, char*, char*, char*);
extern void ecc_description(EllipticCurve);
extern Point ecc_add(EllipticCurve, Point, Point);
extern Point ecc_mul(EllipticCurve, mpz_t, Point);
extern int _lambda(mpz_t, EllipticCurve, Point, Point);

#endif
