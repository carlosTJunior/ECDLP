#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pollardrho.h"

void init_random_number_generator()
{
    srand(time(NULL));
    int seed = rand();
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

int main()
{
    init_random_number_generator();
    EllipticCurve ec = ecc_create("229", "1", "44", "239");
    ecc_description(ec);
    Point *P = point_create("5", "116");
    Point *Q = point_create("155", "166");
    Point *R = ecc_add(ec, P, Q);
    mpz_t num;
    mpz_init_set_ui(num, 17);
    Point *X = ecc_mul(ec, num, P);

    gmp_printf("P(%Zd, %Zd) and Q(%Zd, %Zd)\n", P->x, P->y, Q->x, Q->y);
    gmp_printf("P+Q = (%Zd, %Zd)\n", R->x, R->y);
    gmp_printf("%Zd*P = (%Zd, %Zd)\n", X->x, X->y);

    mpz_t result;
    pollardrho_serial(result, ec, P, Q);
    gmp_printf("Result is %Zd\n", result);
    gmp_randclear(state);

    return 0;
}
