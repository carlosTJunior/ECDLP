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
    EllipticCurve ec = ecc_create("23", "9", "27", "27");
    ecc_description(ec);
    Point P = point_create("16", "5");
    Point Q = point_create("4", "5");

    gmp_printf("P(%Zd, %Zd)\tQ(%Zd, %Zd)\n", P.x, P.y, Q.x, Q.y);
    Point Radd = ecc_add(ec, P, Q);
    gmp_printf("P + Q = (%Zd, %Zd)\n", Radd.x, Radd.y);

    mpz_t num;
    mpz_init_set_ui(num, 9);
    Point PP = ecc_mul(ec, num, P);
    gmp_printf("%Zd P = (%Zd, %Zd)\n", num, PP.x, PP.y);

    pollardrho_serial(num, ec, P, Q);
    gmp_randclear(state);
    return 0;
}
