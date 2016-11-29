#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "pollardrho.h"

using namespace std;

BigInt pollardrho_serial(const EllipticCurve ec, 
                         const Point P,
                         const Point Q, 
                         void (*iteration)(const EllipticCurve ec,
                                           BigInt& c,
                                           BigInt& d,
                                           Point& X,
                                           const Triple* branches,
                                           const unsigned long i))
{
    Triple branches[L];
    int i;
    BigInt result;

    wtc_change_watch(0);

    init_branches(branches, ec, P, Q);

    wtc_change_watch(6);
    /* Floyd cycle detection algorithm */
    /* c1, d1 and X1 are Tortoise vars, c2, d2 and X2 are Hare vars */
    Point X1, X2;
    BigInt c1, d1, c2, d2; 

    wtc_change_watch(1);
    c1 = random_number(ec.order);
    d1 = random_number(ec.order);
    wtc_change_watch(6);
    c2 = c1;
    d2 = d1;

    Point Ptemp, Qtemp;
    ecc_mul(Ptemp, ec, c1, P);
    ecc_mul(Qtemp, ec, d1, Q);
    ecc_add(X1, ec, Ptemp, Qtemp); /* X1 = cP + dQ */
    ecc_add(X2, ec, Ptemp, Qtemp); /* X2 = X1 */


    int has_collided = 0;
    unsigned long j;

    while(!has_collided) {
        wtc_change_watch(2);
        j = partition_function(X1);
        wtc_change_watch(3);
        (*iteration)(ec, c1, d1, X1, branches, j);
        wtc_change_watch(6);

        for(i = 0; i < 2; i++) {
            wtc_change_watch(2);
            j = partition_function(X2);
            wtc_change_watch(3);
            (*iteration)(ec, c2, d2, X2, branches, j);
            wtc_change_watch(6);
        }
        if(X1 == X2)
        {
            printf("---------------------------------------------\n");
            cout << "Collision found at point\n";
            cout << "(" << X1.x << ", " << X1.y << ")\n";
            cout << "With values \n";
            cout << "\tc1 = " << c1 << ", d1 = " << d1 << "\n\tc2 = " << c2 << ", d2 = " << d2 << endl;
            has_collided = 1;
        }
    }
    wtc_change_watch(4);
    result = calculate_result(c1, c2, d1, d2, ec.order);
    wtc_change_watch(6);
    
    return result;
}
