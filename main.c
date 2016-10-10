#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pollardrho.h"

void init_random_number_generator()
{
    srand(time(NULL));
    int seed = rand();

    /* This state is a global variable declared in pollardrho.h */
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

int main(int argc, char* argv[])
{
    init_random_number_generator();
    EllipticCurve ec = ecc_create("229", "1", "44", "239");
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, "5", "116");
    Point *Q = point_alloc();
    point_init(Q, "155", "166");

    /*
    EllipticCurve ec = ecc_create("69234577397554139",\
            "64326",\
            "11751",\
            "69234577237507391");
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, "39361571180675947", "7991682211253487");
    Point *Q = point_alloc();
    point_init(Q, "51992249945632156", "48952372232107871");
    */

    mpz_t result;
    pollardrho_serial(result, ec, P, Q);
    gmp_printf("Result is %Zd\n", result);
    gmp_randclear(state);
    point_destroy(P);
    point_destroy(Q);
    //point_destroy(R);

    return 0;
}
