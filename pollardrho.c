#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

/* Global state to generate random numbers */
//gmp_randstate_t state;

BigInt calculate_result(const BigInt c1, const BigInt c2, \
        const BigInt d1, const BigInt d2, const BigInt order)
{
    BigInt numerator, denominator, result;
    if(d1 == d2) {
        printf("Cannot calculate the discrete log\n");
        return 1;
    }

    /* numerator = (c1 - c2) mod order */
    numerator = (c1 - c2) % order;
    if(numerator < 0) numerator += order;

    /* denominator (d2 - d1)^(-1) mod order */
    denominator = (d2 - d1) % order;
    if(denominator < 0) denominator += order;
    denominator = modInverse(denominator, order);

    /* result */
    result = (numerator * denominator) % order;

    return result;
}

/* this function returns P.x % L */
BigInt partition_function(const Point* P) {
    BigInt result;
    result = P->x % L;
    return result;
}

BigInt random_number(const BigInt max)
{
    /* OBS: rand() returns integer not long */
    return (BigInt) rand() % max;
}

int init_branches(Triple *branches,
                  const EllipticCurve ec,
                  const Point* P, 
                  const Point* Q)
{
    BigInt aj, bj;
    int i;
    for(i = 0; i < L; i++) {
        Point* Rj = point_alloc();
        Point* Ptemp = point_alloc();
        Point* Qtemp = point_alloc();

        aj = random_number(ec.order);
        bj = random_number(ec.order);
        ecc_mul(Ptemp, ec, aj, P);
        ecc_mul(Qtemp, ec, bj, Q);
        ecc_add(Rj, ec, Ptemp, Qtemp);
        branches[i].c = aj;
        branches[i].d = bj;
        branches[i].point = *Rj;
    }

    return 0;
}

void r_adding_walk(const EllipticCurve ec,
                   BigInt* c,
                   BigInt* d, 
                   Point* X, 
                   const Triple* branches, 
                   const unsigned long j)
{
    *c = (*c + branches[j].c) % ec.order;
    *d = (*d + branches[j].d) % ec.order;
    ecc_add(X, ec, X, &branches[j].point);
}
