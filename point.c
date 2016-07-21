#include "point.h"

Point point_create(char* x, char* y)
{
    Point P;
    mpz_init_set_str(P.x, x, 10);
    mpz_init_set_str(P.y, y, 10);

    return P;
}

Point point_create_mpz_t(mpz_t x, mpz_t y)
{
    Point P;
    mpz_init_set(P.x, x);
    mpz_init_set(P.y, y);

    return P;
}

Point point_at_infinity()
{
    Point P;
    mpz_init_set_si(P.x, -1);
    mpz_init_set_si(P.y, -1);

    return P;
}

int point_is_equal(Point P, Point Q)
{
    int ret = 0;
    if(mpz_cmp(P.x, Q.x) == 0 && mpz_cmp(P.y, Q.y) == 0)
        ret = 1;

    return ret;
}
