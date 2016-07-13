#include "point.h"

Point point_create(BigInt x, BigInt y)
{
    Point P;
    P.x = x;
    P.y = y;

    return P;
}
