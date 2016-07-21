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
    Point P = point_create("5", "116");
    Point Q = point_create("155", "166");
    mpz_t result;
    pollardrho_serial(result, ec, P, Q);
    gmp_printf("Result is %Zd\n", result);
    gmp_randclear(state);
    return 0;
}
