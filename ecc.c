#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"

int _lambda(mpz_t lambda, const EllipticCurve ec, \
        const Point* P, const Point* Q);
Point* _sumWithInfinityPoint(const Point* P, const Point* Q);

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

Point* ecc_add(const EllipticCurve ec, const Point* P, const Point* Q)
{
    /* if P or Q is a point at infinity */
    if(mpz_cmp_si(P->x, -1) == 0 || mpz_cmp_si(Q->x, -1) == 0) {
        return _sumWithInfinityPoint(P, Q);
    }

    /* if P are the inverse of Q in the elliptic curve */
    if(mpz_cmp(P->x, Q->x) == 0) {
        /* if P->x == Q->x and P->y == 0, then this point has no inverse */
        if(mpz_cmp_ui(P->y, 0) == 0)
            return point_at_infinity();
        mpz_t temp;
        mpz_init(temp);
        mpz_add(temp, P->y, Q->y);
        /* if P->y + Q->y == ec.p, then Q is the inverse of P */
        if(mpz_cmp(temp, ec.p) == 0) {
            mpz_clear(temp);
            return point_at_infinity();
        }
        mpz_clear(temp);
    }
    Point* result;
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

    result = point_create_mpz_t(xVal, yVal);
    /* free space */
    return result;
}

Point* _sumWithInfinityPoint(const Point* P, const Point* Q)
{
    Point* R = (Point*) malloc(sizeof(Point));
    if(P == NULL) {
        fprintf(stderr, \
                "ERROR: _sumWithInfinityPoint() -> Cannot create pointer\n");
        exit(1);
    }

    if(mpz_cmp_si(P->x, -1) == 0) {
        mpz_init_set(R->x, Q->x);
        mpz_init_set(R->y, Q->y);
    } else {
        mpz_init_set(R->x, P->x);
        mpz_init_set(R->y, P->y);
    }
    return R;
}

Point* ecc_mul(const EllipticCurve ec, const mpz_t n, const Point* P)
{
    Point *result;
    Point *tempQ = point_copy(P);
    mpz_t num; /* copy of n */
    mpz_init_set(num, n);

    /* mpz_divisible_ui_p(a,b) func returns non-zero if b divides a */
    if(mpz_divisible_ui_p(num, 2)){ /* n is even */
        result = point_at_infinity();
    } else {
        result = point_copy(P);
    }
    mpz_tdiv_q_ui(num, num, 2);
    while(mpz_cmp_ui(num, 0) != 0)
    {
        tempQ = ecc_add(ec, tempQ, tempQ);

        if(!mpz_divisible_ui_p(num, 2))
        {
            result = ecc_add(ec, result, tempQ);
        }
        /* integer division of n by 2 */
        mpz_tdiv_q_ui(num, num, 2);
    }
    
    /* free space */
    point_destroy(tempQ);
    mpz_clear(num);
    return result;
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
