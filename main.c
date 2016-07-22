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
    /*
    EllipticCurve ec = ecc_create("229", "1", "44", "239");
    ecc_description(ec);
    Point *P = point_alloc();
    point_create(P, "5", "116");
    Point *Q = point_alloc();
    point_create(Q, "155", "166");
    Point *R = point_alloc();
    ecc_add(R, ec, P, Q);
    mpz_t num;
    mpz_init_set_ui(num, 17);
    Point *X = point_alloc();
    ecc_mul(X, ec, num, P);

    gmp_printf("P(%Zd, %Zd) and Q(%Zd, %Zd)\n", P->x, P->y, Q->x, Q->y);
    gmp_printf("P+Q = (%Zd, %Zd)\n", R->x, R->y);
    gmp_printf("%Zd*P = (%Zd, %Zd)\n", X->x, X->y);
    */
    EllipticCurve ec = ecc_create("69234577397554139",\
            "64326",\
            "11751",\
            "69234577237507391");
    ecc_description(ec);
    Point *P = point_alloc();
    point_create(P, "39361571180675947", "7991682211253487");
    Point *Q = point_alloc();
    point_create(Q, "51992249945632156", "48952372232107871");

    mpz_t result;
    pollardrho_serial(result, ec, P, Q);
    gmp_printf("Result is %Zd\n", result);
    gmp_randclear(state);
    point_destroy(P);
    point_destroy(Q);
    //point_destroy(R);

    return 0;
}
