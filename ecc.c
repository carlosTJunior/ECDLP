#include <stdio.h>
#include "ecc.h"

BigInt _inverse(BigInt num, BigInt p);
BigInt _lambda(EllipticCurve ec, Point P, Point Q);

EllipticCurve ecc_create(BigInt p, BigInt a, BigInt b, BigInt order)
{
    EllipticCurve ec;
    ec.p = p;
    ec.a = a;
    ec.b = b;
    ec.order = order;

    return ec;
}


void ecc_description(EllipticCurve ec)
{
    printf("E(F_%ld): y^2 = x^3 + %ldx + %ld, #E(F_%ld) = %ld\n", \
            ec.p, ec.a, ec.b, ec.p, ec.order);
}

Point ecc_add(EllipticCurve ec, Point P, Point Q)
{
    Point result;
    BigInt l = _lambda(ec, P, Q);

    result.x = (l * l - P.x - Q.x) % ec.p;
    result.y = (l * (P.x - result.x) - P.y) % ec.p;
    return result;
}

Point ecc_mul(EllipticCurve ec, BigInt n, Point P)
{
    Point result;

    return result;
}

BigInt _lambda(EllipticCurve ec, Point P, Point Q)
{
    BigInt lambda;

    if(P.x == Q.x && P.y == Q.y) {
        lambda = ((3 * P.x * P.x + ec.a) * _inverse(2 * P.y, ec.p)) % ec.p;
    } else {
        lambda = ((Q.y - P.y) * _inverse(Q.x - P.x, ec.p)) % ec.p;
    }

    return lambda;
}

BigInt _inverse(BigInt num, BigInt p)
{
    BigInt d, i, r;
    for(i = 0; i < p; i++ ) {
        r = (d * num) % p;
        if(r == 1)
            break;
    }
    if(i == p) {
        printf("%ld has no inverse modulo %ld\n", num, p);
        return -1;
    }
    return d;
}
