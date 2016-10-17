#ifndef _ECC
#define _ECC

/*#define DEBUG*/

#include "point.h"

typedef struct _ecc {
    BigInt p;
    BigInt a;
    BigInt b;
    BigInt order;
} EllipticCurve;

extern EllipticCurve ecc_create(const BigInt p, 
                                const BigInt a, 
                                const BigInt b,
                                const BigInt order);

extern void ecc_description(const EllipticCurve ec);

extern int ecc_add(Point* R,
                   const EllipticCurve ec,
                   const Point* P, 
                   const Point* Q);

extern int ecc_mul(Point* R, 
                   const EllipticCurve ec, 
                   const BigInt, 
                   const Point* P);

extern int ecc_halving(Point* R,
                       const EllipticCurve ec,
                       const Point* P);


extern BigInt _lambda(const EllipticCurve ec, 
                      const Point* P, 
                      const Point* Q);

extern BigInt modInverse(BigInt a, BigInt m);

#endif
