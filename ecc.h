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

extern EllipticCurve ecc_create(const char*, const char*, const char*,\
       const char*);

extern void ecc_description(const EllipticCurve);
extern int ecc_add(Point*, const EllipticCurve, const Point*, const Point*);
extern int ecc_mul(Point*, const EllipticCurve, const mpz_t, const Point*);
extern int ecc_halving(Point*, const EllipticCurve, const Point*);
extern int _lambda(mpz_t, const EllipticCurve, const Point*, const Point*);

#endif
