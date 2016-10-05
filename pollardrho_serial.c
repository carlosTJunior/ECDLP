#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"



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

    Point* Ptemp = point_alloc(); /* cP */
    ecc_mul(Ptemp, ec, c1, P);
    Point* Qtemp = point_alloc(); /* dQ */
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
        ecc_add(X1, ec, X1, &branches[j].point);

        for(i = 0; i < 2; i++) {
            j = partition_function(X2);
            mpz_add(c2, c2, branches[j].c);
            mpz_add(d2, d2, branches[j].d);
            mpz_mod(c2, c2, ec.order);
            mpz_mod(d2, d2, ec.order);
            ecc_add(X2, ec, X2, &branches[j].point);
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
