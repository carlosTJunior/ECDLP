#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pollardrho.h"

void init_random_number_generator()
{
    srand(time(NULL));
    int seed = rand() + 1;
}

int main(int argc, char* argv[])
{
    int opt;

    /* Pointer to a iteration function to be used */
    void (*iteration_function)(const EllipticCurve,
                               BigInt*, BigInt*, Point*,
                               const Triple*,
                               const unsigned long);

    /* Pointer to a Pollard Rho algorithm version to be used */
    BigInt (*pollard_algorithm)(const EllipticCurve ec, 
                                const Point* P,
                                const Point* Q, 
                                void (*iteration)(const EllipticCurve ec,
                                                  BigInt* c,
                                                  BigInt* d,
                                                  Point* X,
                                                  const Triple* branches,
                                                  const unsigned long i));

    iteration_function = NULL;
    pollard_algorithm = NULL;

    while( (opt = getopt(argc, argv, "+a:w:")) != -1) {
        switch(opt) {
        case 'w': /* walk (iteration) function */
            /* r = r_adding_walk */
            if(strcmp(optarg, "r") == 0) {
                iteration_function = r_adding_walk;
            }
            break;

        case 'a': /* algorithm */
            /* serial pollard rho */
            if(strcmp(optarg, "serial") == 0) {
                pollard_algorithm = pollardrho_serial;
            }
            /* parallel using fork */
            else if(strcmp(optarg, "fork") == 0) {
                pollard_algorithm = pollardrho_parallel_fork;
            }
            break;

        default:
            iteration_function = r_adding_walk;
            pollard_algorithm = pollardrho_serial;
        }
    }
    printf("\n\n");
    if(iteration_function == NULL) {
        fprintf(stdout, "Iteration function not selected. Using r-adding-walk as default\n");
        iteration_function = r_adding_walk;
    }
    if(pollard_algorithm == NULL) {
        fprintf(stdout, "Algorithm not selected: Using 'Serial' as default\n");
        pollard_algorithm = pollardrho_serial;
    }

    init_random_number_generator();

    /*------------------------------ setting Curves and ECDLP Points -----------------------------*/
    /*
    EllipticCurve ec = ecc_create(229, 1, 44, 239);
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, 5, 116);
    Point *Q = point_alloc();
    point_init(Q, 155, 166);

    EllipticCurve ec = ecc_create(69234577397554139,
                                  64326,
                                  11751,
                                  69234577237507391);
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, 39361571180675947, 7991682211253487);
    Point *Q = point_alloc();
    point_init(Q, 51992249945632156, 48952372232107871);
    */

    EllipticCurve ec = ecc_create(2879867477,
                                  62293,
                                  47905,
                                  2879882063);
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, 1482193291, 1063050205);
    Point *Q = point_alloc();
    point_init(Q, 2146105060, 1451020666);

    /*
    EllipticCurve ec = ecc_create(7919,
                                  1001,
                                  75,
                                  7889);
    ecc_description(ec);
    Point *P = point_alloc();
    point_init(P, 4023, 6036);
    Point *Q = point_alloc();
    point_init(Q, 4135, 3169);
    */
    /*-------------------------------- Curves and Points ending setting ---------------------------*/

    BigInt result;

    result = (*pollard_algorithm)(ec, P, Q, iteration_function);

    printf("Result is %lld\n", result);
    point_destroy(P);
    point_destroy(Q);

    return 0;
}
