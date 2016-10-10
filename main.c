#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    int opt;
    void (*iteration_function)(const EllipticCurve,
                               mpz_t, mpz_t, Point*,
                               const Triple*,
                               const unsigned long);

    iteration_function = NULL;
    while( (opt = getopt(argc, argv, "+w:")) != -1) {
        switch(opt) {
        case 'w': 
            if(strcmp(optarg, "r") == 0) {
                iteration_function = r_adding_walk;
            }
            break;

        default:
            iteration_function = r_adding_walk;
        }
    }
    printf("\n\n");
    if(iteration_function == NULL) {
        fprintf(stdout, "Iteration function not selected. Using r-adding-walk as default\n");
        iteration_function = r_adding_walk;
    }

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
    pollardrho_serial(result, ec, P, Q, iteration_function);
    gmp_printf("Result is %Zd\n", result);
    gmp_randclear(state);
    point_destroy(P);
    point_destroy(Q);
    //point_destroy(R);

    return 0;
}
