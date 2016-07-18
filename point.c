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
