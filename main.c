#include <stdio.h>
#include "ecc.h"
#include "point.h"


int main()
{
    EllipticCurve ec = ecc_create("23", "1", "1", "27");
    ecc_description(ec);
    Point P = point_create("3", "10");
    Point Q = point_create("9", "7");

    Point Radd = ecc_add(ec, P, Q);
    gmp_printf("P + Q = (%Zd, %Zd)\n", Radd.x, Radd.y);

    return 0;
}
