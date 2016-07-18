#include <stdio.h>
#include "ecc.h"

int _lambda(mpz_t lambda, EllipticCurve ec, Point P, Point Q);
Point _sumWithInfinityPoint(Point P, Point Q);

EllipticCurve ecc_create(char* p, char* a, char* b, char* order)
{
    EllipticCurve ec;
    mpz_init_set_str(ec.p, p, 10); /* 10 means decimal base */
    mpz_init_set_str(ec.a, a, 10);
    mpz_init_set_str(ec.b, b, 10);
    mpz_init_set_str(ec.order, order, 10);

    return ec;
}

void ecc_description(EllipticCurve ec)
{
    gmp_printf("E(F_%Zd): y^2 = x^3 + %Zdx + %Zd, #E(F_%Zd) = %Zd\n", \
            ec.p, ec.a, ec.b, ec.p, ec.order);
}

Point ecc_add(EllipticCurve ec, Point P, Point Q)
{
    if(mpz_cmp_si(P.x, -1) == 0 || mpz_cmp_si(Q.x, -1) == 0) {
        return _sumWithInfinityPoint(P, Q);
    }

    Point result;
    mpz_t xVal, yVal, lambda;
    mpz_init(xVal);
    mpz_init(yVal);
    mpz_init(lambda);

    /* calculates lambda */
    _lambda(lambda, ec, P, Q);

    /* calculates x */
    mpz_mul(xVal, lambda, lambda);
    mpz_sub(xVal, xVal, P.x);
    mpz_sub(xVal, xVal, Q.x);
    mpz_mod(xVal, xVal, ec.p);

    /* calculates y */
    mpz_sub(yVal, P.x, xVal);
    mpz_mul(yVal, yVal, lambda);
    mpz_sub(yVal, yVal, P.y);
    mpz_mod(yVal, yVal, ec.p);

    result = point_create_mpz_t(xVal, yVal);
    /* free space */
    return result;
}

Point _sumWithInfinityPoint(Point P, Point Q)
{
    if(mpz_cmp_si(P.x, -1) == 0)
        return Q;
    else
        return P;
}

Point ecc_mul(EllipticCurve ec, mpz_t n, Point P)
{
    Point result, tempQ;
    tempQ = P;
/*
    while(mpz_cmp_ui(n, 0) != 0)
    {
        if(!mpz_divisible_ui_p(n, 2))
        {
            
        }
    }
    */
    /* free space */
    return result;
}

int _lambda(mpz_t lambda, EllipticCurve ec, Point P, Point Q)
{
    #ifdef DEBUG
    printf("Will calculate lambda for\n");
    ecc_description(ec);
    gmp_printf("Point P(%Zd, %Zd)\n", P.x, P.y);
    gmp_printf("Point Q(%Zd, %Zd)\n", Q.x, Q.y);
    #endif
    mpz_t denominator, numerator;
    mpz_init(denominator);
    mpz_init(numerator);

    if(mpz_cmp(P.x, Q.x) == 0 && mpz_cmp(P.y, Q.y) == 0) {
        /* denominator */
        mpz_mul_si(denominator, P.y, 2);
        mpz_invert(denominator, denominator, ec.p);

        /* numerator */
        mpz_mul(numerator, P.x, P.x);
        mpz_mul_ui(numerator, numerator, 3);
        mpz_add(numerator, numerator, ec.a);
        mpz_mod(numerator, numerator, ec.p);
    } else {
        /* denominator */
        mpz_sub(denominator, Q.x, P.x);
        mpz_invert(denominator, denominator, ec.p);

        /* numerator */
        mpz_sub(numerator, Q.y, P.y);
    }
    /* lambda */
    mpz_set(lambda, numerator);
    mpz_mul(lambda, lambda, denominator);
    mpz_mod(lambda, lambda, ec.p);

    /* free space */
    return 0;
}
