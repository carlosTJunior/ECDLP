#ifndef _ECC
#define _ECC

/*#define DEBUG*/

#include "point.h"
#include "watches.h"

struct EllipticCurve {
    BigInt p;
    BigInt a;
    BigInt b;
    BigInt order;

    EllipticCurve() {}
    EllipticCurve(const BigInt p,
                  const BigInt a,
                  const BigInt b,
                  const BigInt order) :
        p(p), a(a), b(b), order(order) {}
};


extern void ecc_description(const EllipticCurve ec);

extern int ecc_add(Point& R,
                   const EllipticCurve ec,
                   const Point P, 
                   const Point Q);

extern int ecc_mul(Point& R, 
                   const EllipticCurve ec, 
                   const BigInt, 
                   const Point P);

extern int ecc_halving(Point& R,
                       const EllipticCurve ec,
                       const Point P);


extern BigInt _lambda(const EllipticCurve ec, 
                      const Point P, 
                      const Point Q);

extern BigInt modInverse(BigInt a, BigInt m);

#endif
