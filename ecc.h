#ifndef _ECC
#define _ECC

#include "point.h"

typedef struct _ecc {
    BigInt p;
    BigInt a;
    BigInt b;
    BigInt order;
} EllipticCurve;

extern EllipticCurve ecc_create(BigInt, BigInt, BigInt, BigInt);
extern void ecc_description(EllipticCurve);
extern Point ecc_add(EllipticCurve, Point, Point);
extern Point ecc_mul(EllipticCurve, BigInt, Point);

#endif
