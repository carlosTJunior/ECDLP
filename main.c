#include <stdio.h>
#include "ecc.h"
#include "point.h"

int main()
{
    EllipticCurve ec = ecc_create(23, 1, 1, 27);
    ecc_description(ec);

    Point P, Q;
    P = point_create(3, 10);
    Q = point_create(9, 7);
    /*

    Point Radd = ecc_add(ec, P, Q);
    Point R2 = ecc_add(ec, P, P);

    printf("P + Q = (%ld, %ld)\n", Radd.x, Radd.y);
    printf("2P = (%ld, %ld)\n", R2.x, R2.y);
    */
    return 0;
}
