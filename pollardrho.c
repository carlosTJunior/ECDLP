#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

/* Global state to generate random numbers */
gmp_randstate_t state;

int calculate_result(mpz_t result, const mpz_t c1, const mpz_t c2, \
        const mpz_t d1, const mpz_t d2, const mpz_t order)
{
    mpz_t numerator, denominator;
    if(mpz_cmp(d1, d2) == 0) {
        printf("Cannot calculate the discrete log\n");
        return 1;
    }

    mpz_init_set_ui(result, 0);
    mpz_init(numerator);
    mpz_init(denominator);

    /* numerator = (c1 - c2) mod order */
    mpz_sub(numerator, c1, c2);
    //mpz_mod(numerator, numerator, order);

    /* denominator (d2 - d1)^(-1) mod order */
    mpz_sub(denominator, d2, d1);
    //mpz_mod(denominator, denominator, order);
    mpz_invert(denominator, denominator, order);

    /* result */
    mpz_mul(result, numerator, denominator);
    mpz_mod(result, result, order);

    mpz_clear(denominator);
    mpz_clear(numerator);
    return 0;
}

/* this function returns P.x % L */
unsigned long partition_function(const Point* P) {
    mpz_t result;
    mpz_init(result);
    mpz_mod_ui(result, P->x, L);
    unsigned long ui_result = mpz_get_ui(result);
    mpz_clear(result);
    return ui_result;
}

void random_number(mpz_t result, const mpz_t max)
{
    mpz_t new_max;
    mpz_init(new_max);
    mpz_sub_ui(new_max, max, 1);
    mpz_urandomm(result, state, new_max);
    mpz_clear(new_max);
}

int init_branches(Triple *branches, const EllipticCurve ec, \
       const Point* P, const Point* Q)
{
    mpz_t aj, bj;
    mpz_init(aj);
    mpz_init(bj);
    int i;
    for(i = 0; i < L; i++) {
        Point* Rj = point_alloc();
        Point* Ptemp = point_alloc();
        Point* Qtemp = point_alloc();

        random_number(aj, ec.order);
        random_number(bj, ec.order);
        ecc_mul(Ptemp, ec, aj, P);
        ecc_mul(Qtemp, ec, bj, Q);
        ecc_add(Rj, ec, Ptemp, Qtemp);
        mpz_init_set(branches[i].c, aj);
        mpz_init_set(branches[i].d, bj);
        branches[i].point = *Rj;
    }

    mpz_clear(aj);
    mpz_clear(bj);
    return 0;
}
