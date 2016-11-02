#include "point.h"

void point_at_infinity(Point& P)
{
    P.x = -1;
    P.y = -1;
}

bool operator==(const Point P, const Point Q)
{
    return P.x == Q.x && P.y == Q.y;
}
