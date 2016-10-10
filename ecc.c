#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"

int _lambda(mpz_t lambda, const EllipticCurve ec, \
        const Point* P, const Point* Q);
int _sumWithInfinityPoint(Point* R, const Point* P, const Point* Q);

EllipticCurve 
ecc_create(const char* p, const char* a, const char* b, const char* order)
{
    EllipticCurve ec;
    mpz_init_set_str(ec.p, p, 10); /* 10 means decimal base */
    mpz_init_set_str(ec.a, a, 10);
    mpz_init_set_str(ec.b, b, 10);
    mpz_init_set_str(ec.order, order, 10);

    return ec;
}

void ecc_description(const EllipticCurve ec)
{
    gmp_printf("E(F_%Zd): y^2 = x^3 + %Zdx + %Zd, #E(F_%Zd) = %Zd\n", \
            ec.p, ec.a, ec.b, ec.p, ec.order);
}

int ecc_add(Point *R, const EllipticCurve ec, const Point* P, const Point* Q)
{
    if(R == NULL) {
        fprintf(stderr, "ERROR: ecc_add -> invalid pointer\n");
        return -1;
    }
    /* if P or Q is a point at infinity */
    if(mpz_cmp_si(P->x, -1) == 0 || mpz_cmp_si(Q->x, -1) == 0) {
        _sumWithInfinityPoint(R, P, Q);
        return 0;
    }

    /* if P are the inverse of Q in the elliptic curve */
    if(mpz_cmp(P->x, Q->x) == 0) {
        /* if P->x == Q->x and P->y == 0, then this point has no inverse */
        if(mpz_cmp_ui(P->y, 0) == 0) {
            point_at_infinity(R);
            return 0;
        }
        mpz_t temp;
        mpz_init(temp);
        mpz_add(temp, P->y, Q->y);
        /* if P->y + Q->y == ec.p, then Q is the inverse of P */
        if(mpz_cmp(temp, ec.p) == 0) {
            mpz_clear(temp);
            point_at_infinity(R);
            return 0;
        }
        mpz_clear(temp);
    }
    mpz_t xVal, yVal, lambda;
    mpz_init(xVal);
    mpz_init(yVal);
    mpz_init(lambda);

    /* calculates lambda */
    _lambda(lambda, ec, P, Q);

    /* calculates x */
    mpz_mul(xVal, lambda, lambda);
    mpz_sub(xVal, xVal, P->x);
    mpz_sub(xVal, xVal, Q->x);
    mpz_mod(xVal, xVal, ec.p);

    /* calculates y */
    mpz_sub(yVal, P->x, xVal);
    mpz_mul(yVal, yVal, lambda);
    mpz_sub(yVal, yVal, P->y);
    mpz_mod(yVal, yVal, ec.p);

    point_init_mpz_t(R, xVal, yVal);
    /* free space */
    mpz_clear(xVal);
    mpz_clear(yVal);
    mpz_clear(lambda);
    return 0;
}

int _sumWithInfinityPoint(Point *R, const Point* P, const Point* Q)
{
    if(R == NULL) {
        fprintf(stderr, \
                "ERROR: _sumWithInfinityPoint() -> invalid pointer\n");
        return -1;
    }

    if(mpz_cmp_si(P->x, -1) == 0) {
        mpz_set(R->x, Q->x);
        mpz_set(R->y, Q->y);
    } else {
        mpz_set(R->x, P->x);
        mpz_set(R->y, P->y);
    }
    return 0;
}

int ecc_mul(Point* R, const EllipticCurve ec, const mpz_t n, const Point* P)
{
    if(R == NULL) {
        fprintf(stderr, "ERROR: ecc_mul() -> invalid pointer\n");
        return -1;
    }
    Point *tempQ = point_alloc();
    point_copy(tempQ, P);
    mpz_t num; /* copy of n */
    mpz_init_set(num, n);

    /* mpz_divisible_ui_p(a,b) func returns non-zero if b divides a */
    if(mpz_divisible_ui_p(num, 2)){ /* n is even */
        point_at_infinity(R);
    } else {
        point_copy(R, P);
    }
    mpz_tdiv_q_ui(num, num, 2);
    while(mpz_cmp_ui(num, 0) != 0)
    {
        ecc_add(tempQ, ec, tempQ, tempQ);

        if(!mpz_divisible_ui_p(num, 2))
        {
            ecc_add(R, ec, R, tempQ);
        }
        /* integer division of n by 2 */
        mpz_tdiv_q_ui(num, num, 2);
    }
    
    /* free space */
    point_destroy(tempQ);
    mpz_clear(num);
    return 0;
}

int ecc_halving(Point* R, const EllipticCurve ec, const Point* P)
{
    /* yet to implement */
    return 0;
}

int
_lambda(mpz_t lambda, const EllipticCurve ec, const Point* P, const Point* Q)
{
    #ifdef DEBUG
    printf("Will calculate lambda for\n");
    ecc_description(ec);
    gmp_printf("Point P(%Zd, %Zd)\n", P->x, P->y);
    gmp_printf("Point Q(%Zd, %Zd)\n", Q->x, Q->y);
    #endif
    mpz_t denominator, numerator;
    mpz_init(denominator);
    mpz_init(numerator);

    if(mpz_cmp(P->x, Q->x) == 0 && mpz_cmp(P->y, Q->y) == 0) {
        /* denominator */
        mpz_mul_si(denominator, P->y, 2);
        mpz_invert(denominator, denominator, ec.p);

        /* numerator */
        mpz_mul(numerator, P->x, P->x);
        mpz_mul_ui(numerator, numerator, 3);
        mpz_add(numerator, numerator, ec.a);
        mpz_mod(numerator, numerator, ec.p);
    } else {
        /* denominator */
        mpz_sub(denominator, Q->x, P->x);
        mpz_invert(denominator, denominator, ec.p);

        /* numerator */
        mpz_sub(numerator, Q->y, P->y);
    }
    /* lambda */
    mpz_set(lambda, numerator);
    mpz_mul(lambda, lambda, denominator);
    mpz_mod(lambda, lambda, ec.p);

    /* free space */
    mpz_clear(denominator);
    mpz_clear(numerator);
    return 0;
}
