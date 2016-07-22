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
    Point *X1 = point_alloc();
    Point *X2 = point_alloc();
    mpz_t c1, d1, c2, d2; 
    mpz_init(c1);
    mpz_init(d1);
    mpz_init(c2);
    mpz_init(d2);

    random_number(c1, ec.order);
    random_number(d1, ec.order);
    mpz_set(c2, c1);
    mpz_set(d2, d1);

    Point* Ptemp = point_alloc();
    ecc_mul(Ptemp, ec, c1, P);
    Point* Qtemp = point_alloc();
    ecc_mul(Qtemp, ec, d1, Q);
    ecc_add(X1, ec, Ptemp, Qtemp); /* X1 = cP + dQ */
    ecc_add(X2, ec, Ptemp, Qtemp); /* X2 = X1 */

    int has_collided = 0;
    unsigned long j;
    while(!has_collided) {
        //gmp_printf("X1(%Zd, %Zd)\t", X1->x, X1->y);
        j = partition_function(X1);
        mpz_add(c1, c1, branches[j].c);
        mpz_add(d1, d1, branches[j].d);
        mpz_mod(c1, c1, ec.order);
        mpz_mod(d1, d1, ec.order);
        ecc_add(X1, ec, X1, branches[j].point);

        for(i = 0; i < 2; i++) {
            j = partition_function(X2);
            mpz_add(c2, c2, branches[j].c);
            mpz_add(d2, d2, branches[j].d);
            mpz_mod(c2, c2, ec.order);
            mpz_mod(d2, d2, ec.order);
            ecc_add(X2, ec, X2, branches[j].point);
        }
        //gmp_printf("X2(%Zd, %Zd)\n", X2->x, X2->y);
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
    
    mpz_clear(c1);
    mpz_clear(c2);
    mpz_clear(d1);
    mpz_clear(d2);
    point_destroy(Ptemp);
    point_destroy(Qtemp);
    point_destroy(X1);
    point_destroy(X2);
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
        branches[i].point = Rj;
    }

    mpz_clear(aj);
    mpz_clear(bj);
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

