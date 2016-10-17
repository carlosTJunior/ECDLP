#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

BigInt pollardrho_serial(const EllipticCurve ec, 
                         const Point* P,
                         const Point* Q, 
                         void (*iteration)(const EllipticCurve ec,
                                           BigInt* c,
                                           BigInt* d,
                                           Point* X,
                                           const Triple* branches,
                                           const unsigned long i))
{
    Triple branches[L];
    int i;
    BigInt result;

    init_branches(branches, ec, P, Q);

    /* Floyd cycle detection algorithm */
    /* c1, d1 and X1 are Tortoise vars, c2, d2 and X2 are Hare vars */
    Point *X1 = point_alloc();
    Point *X2 = point_alloc();
    BigInt c1, d1, c2, d2; 

    c1 = random_number(ec.order);
    d1 = random_number(ec.order);
    c2 = c1;
    d2 = d1;

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
        (*iteration)(ec, &c1, &d1, X1, branches, j);

        for(i = 0; i < 2; i++) {
            j = partition_function(X2);
            (*iteration)(ec, &c2, &d2, X2, branches, j);
        }
        //gmp_printf("X2(%Zd, %Zd)\n", X2->x, X2->y);
        if(point_is_equal(X1, X2))
        {
            printf("---------------------------------------------\n");
            printf("Collision found at point\n");
            printf("X1(%ld, %ld) X2(%ld, %ld)\n", 
                    X1->x, X1->y, X2->x, X2->y);
            printf("With values \n");
            printf("c1 = %ld, d1 = %ld and c2 = %ld, d2 = %ld\n", \
                    c1, d1, c2, d2);
            has_collided = 1;
        }
    }
    result = calculate_result(c1, c2, d1, d2, ec.order);
    
    point_destroy(Ptemp);
    point_destroy(Qtemp);
    point_destroy(X1);
    point_destroy(X2);
    return result;
}
