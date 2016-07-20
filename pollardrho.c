#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

/* number of branches */
#define L 32
gmp_randstate_t state;

int init_branches(Triple *branches, EllipticCurve ec, Point P, Point Q);
unsigned long int partition_function(Point P);
void random_number(mpz_t result, mpz_t max);

int
pollardrho_serial(mpz_t result, EllipticCurve ec, Point P, Point Q)
{
    Triple branches[L];
    int i;
    init_branches(branches, ec, P, Q);

    for(i = 0; i < L; i++) {
        gmp_printf("c = %Zd, d = %Zd, R(%Zd, %Zd)\n", branches[i].c, \
                branches[i].d, branches[i].point.x, branches[i].point.y);
    }
    return 0;
}

/* this function returns P.x % L */
unsigned long int partition_function(Point P) {
    mpz_t result;
    mpz_init(result);
    mpz_mod_ui(result, P.x, L);

    return mpz_get_ui(result);
}

void random_number(mpz_t result, mpz_t max)
{
    /*
    gmp_randstate_t st;
    gmp_randinit_mt(st);
    */
    mpz_urandomm(result, state, max);
}

int init_branches(Triple *branches, EllipticCurve ec, Point P, Point Q)
{
    mpz_t aj, bj;
    mpz_init(aj);
    mpz_init(bj);
    int i;
    for(i = 0; i < L; i++) {
        Point Rj, Ptemp, Qtemp;
        random_number(aj, ec.p);
        random_number(bj, ec.p);
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

