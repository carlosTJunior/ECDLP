#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"

int _sumWithInfinityPoint(Point* R, const Point* P, const Point* Q);

EllipticCurve 
ecc_create(const BigInt p, const BigInt a, const BigInt b, const BigInt order)
{
    EllipticCurve ec;
    ec.p = p;
    ec.a = a;
    ec.b = b;
    ec.order = order;

    return ec;
}

void ecc_description(const EllipticCurve ec)
{
    printf("E(F_%ld): y^2 = x^3 + %ldx + %ld, #E(F_%ld) = %ld\n", 
            ec.p, ec.a, ec.b, ec.p, ec.order);
}

int ecc_add(Point *R, const EllipticCurve ec, const Point* P, const Point* Q)
{
    if(R == NULL) {
        fprintf(stderr, "ERROR: ecc_add -> invalid pointer\n");
        return -1;
    }
    /* if P or Q is a point at infinity */
    if((P->x == -1) || (Q->x == -1)) {
        _sumWithInfinityPoint(R, P, Q);
        return 0;
    }

    /* if P are the inverse of Q in the elliptic curve */
    if(P->x == Q->x) {
        /* if P->x == Q->x and P->y == 0, then this point has no inverse */
        if(P->y == 0) {
            point_at_infinity(R);
            return 0;
        }
        BigInt temp;
        temp = P->y + Q->y;
        /* if P->y + Q->y == ec.p, then Q is the inverse of P */
        if(temp == ec.p) {
            point_at_infinity(R);
            return 0;
        }
    }
    BigInt xVal, yVal, lambda;

    /* calculates lambda */
    lambda = _lambda(ec, P, Q);

    /* calculates x */
    xVal = (lambda * lambda - P->x - Q->x) % ec.p;
    if(xVal < 0) xVal += ec.p;

    /* calculates y */
    yVal = ((P->x - xVal) * lambda - P->y) % ec.p;
    if(yVal < 0) yVal += ec.p;

    point_init(R, xVal, yVal);

    return 0;
}

int _sumWithInfinityPoint(Point *R, const Point* P, const Point* Q)
{
    if(R == NULL) {
        fprintf(stderr, "ERROR: _sumWithInfinityPoint() -> invalid pointer\n");
        return -1;
    }

    /* if P is point at infinity, then R = Q, else R = P */
    if(P->x == -1) {
        point_copy(R, Q);
    } else {
        point_copy(R, P);
    }
    return 0;
}

int ecc_mul(Point* R, const EllipticCurve ec, const BigInt n, const Point* P)
{
    if(R == NULL) {
        fprintf(stderr, "ERROR: ecc_mul() -> invalid pointer\n");
        return -1;
    }
    Point *tempQ = point_alloc();
    point_copy(tempQ, P);
    BigInt num; /* copy of n */
    num = n;

    if(num % 2 == 0) { /* n is even */
        point_at_infinity(R);
    } else {
        point_copy(R, P);
    }
    num /= 2;
    while(num != 0)
    {
        ecc_add(tempQ, ec, tempQ, tempQ);

        if(num % 2 != 0)
        {
            ecc_add(R, ec, R, tempQ);
        }
        /* integer division of n by 2 */
        num /= 2;
    }
    
    /* free space */
    point_destroy(tempQ);
    return 0;
}

int ecc_halving(Point* R, const EllipticCurve ec, const Point* P)
{
    /* yet to implement */
    return 0;
}

BigInt
_lambda(const EllipticCurve ec, const Point* P, const Point* Q)
{
    BigInt lambda;
    BigInt denominator, numerator;

    if(point_is_equal(P, Q)) {
        denominator = P->y * 2;
        numerator = (3 * (P->x * P->x) + ec.a) % ec.p;
    } else {
        denominator = Q->x - P->x;
        numerator = (Q->y - P->y) % ec.p;
    }

    if(denominator < 0) denominator += ec.p;
    denominator = modInverse(denominator, ec.p);

    if(numerator < 0) numerator += ec.p;

    lambda = (numerator * denominator) % ec.p; /* OBS: denominator is inverted */

    return lambda;
}

/* ------------------ Math functions to calculate inverse --------------*/
BigInt modInverse(BigInt a, BigInt m) {
    BigInt t, q, m0 = m;
    BigInt x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        /* q is quotient */
        q = a / m;
        t = m;
        /* m is remainder now, Euclid Algorithm */
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    /* Make x1 positive */
    if (x1 < 0)
        x1 += m0;
    return x1;
}
