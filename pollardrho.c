#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

/* number of branches */
#define L 32

/* Global state to generate random numbers */
gmp_randstate_t state;


/* Functions prototypes */
int init_branches(Triple *branches, const EllipticCurve ec, const Point* P,\
       const Point* Q);
unsigned long int partition_function(const Point* P);
void random_number(mpz_t result, const mpz_t max);
int calculate_result(mpz_t result, const mpz_t c1, const mpz_t c2, \
        const mpz_t d1, const mpz_t d2, const mpz_t order);

int
pollardrho_serial(mpz_t result, const EllipticCurve ec, const Point* P, \
        const Point* Q)
{
    Triple branches[L];
    int i;
    init_branches(branches, ec, P, Q);

    /* Floyd cycle detection algorithm */
    /* c1, d1 and X1 are Tortoise vars, c2, d2 and X2 are Hare vars */
    Point *X1, *X2;
    mpz_t c1, d1, c2, d2; 
    mpz_init(c1);
    mpz_init(d1);
    mpz_init(c2);
    mpz_init(d2);

    random_number(c1, ec.order);
    random_number(d1, ec.order);
    mpz_set(c2, c1);
    mpz_set(d2, d1);

    Point* Ptemp = ecc_mul(ec, c1, P);
    Point* Qtemp = ecc_mul(ec, d1, Q);
    X1 = ecc_add(ec, Ptemp, Qtemp); /* X1 = cP + dQ */
    X2 = ecc_add(ec, Ptemp, Qtemp); /* X2 = X1 */

    int has_collided = 0;
    while(!has_collided) {
        gmp_printf("X1(%Zd, %Zd)\t", X1->x, X1->y);
        int i;
        unsigned long j = partition_function(X1);
        mpz_add(c1, c1, branches[j].c);
        mpz_add(d1, d1, branches[j].d);
        mpz_mod(c1, c1, ec.order);
        mpz_mod(d1, d1, ec.order);
        X1 = ecc_add(ec, X1, branches[j].point);

        for(i = 0; i < 2; i++) {
            j = partition_function(X2);
            mpz_add(c2, c2, branches[j].c);
            mpz_add(d2, d2, branches[j].d);
            mpz_mod(c2, c2, ec.order);
            mpz_mod(d2, d2, ec.order);
            X2 = ecc_add(ec, X2, branches[j].point);
        }
        gmp_printf("X2(%Zd, %Zd)\n", X2->x, X2->y);
        if(point_is_equal(X1, X2))
        {
            printf("Collision found at point\n");
            gmp_printf("X1(%Zd, %Zd)\n", X1->x, X1->y, \
                    X2->x, X2->y);
            printf("With values \n");
            gmp_printf("c1 = %Zd, d1 = %Zd and c2 = %Zd, d2 = %Zd\n", \
                    c1, d1, c2, d2);
            has_collided = 1;
        }
    }
    calculate_result(result, c1, c2, d1, d2, ec.order);
    return 0;
}

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

    return 0;
}

/* this function returns P.x % L */
unsigned long partition_function(const Point* P) {
    mpz_t result;
    mpz_init(result);
    mpz_mod_ui(result, P->x, L);

    return mpz_get_ui(result);
}

void random_number(mpz_t result, const mpz_t max)
{
    mpz_t new_max;
    mpz_init(new_max);
    mpz_sub_ui(new_max, max, 1);
    mpz_urandomm(result, state, new_max);
}

int init_branches(Triple *branches, const EllipticCurve ec, \
       const Point* P, const Point* Q)
{
    mpz_t aj, bj;
    mpz_init(aj);
    mpz_init(bj);
    int i;
    for(i = 0; i < L; i++) {
        Point *Rj, *Ptemp, *Qtemp;
        random_number(aj, ec.order);
        random_number(bj, ec.order);
        Ptemp = ecc_mul(ec, aj, P);
        Qtemp = ecc_mul(ec, bj, Q);
        Rj = ecc_add(ec, Ptemp, Qtemp);
        mpz_init_set(branches[i].c, aj);
        mpz_init_set(branches[i].d, bj);
        branches[i].point = Rj;
    }

    return 0;
}

/* ################ Put into another file ############## */
int
pollardrho_serial_openmp(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_fork(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_socket(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_openmp(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_pthread(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_mpi(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_cuda(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}


int
pollardrho_parallel_opencl(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    return 0;
}

