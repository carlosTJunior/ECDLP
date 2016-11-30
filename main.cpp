#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "pollardrho.h"
#include "random.h"

// Global randclass declared in pollardrho.h
//gmp_randclass randclass(gmp_randinit_mt);

using namespace std;


int main(int argc, char* argv[])
{
    int opt;
    //char filename[32] = {0};

    /* Pointer to a iteration function to be used */
    void (*iteration_function)(const EllipticCurve,
                               BigInt&, BigInt&, Point&,
                               const Triple*,
                               const unsigned long);

    /* Pointer to a Pollard Rho algorithm version to be used */
    BigInt (*pollard_algorithm)(const EllipticCurve ec, 
                                const Point P,
                                const Point Q, 
                                void (*iteration)(const EllipticCurve ec,
                                                  BigInt& c,
                                                  BigInt& d,
                                                  Point& X,
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
            /* parallel using MPI */
            else if(strcmp(optarg, "mpi") == 0) {
                pollard_algorithm = pollardrho_parallel_mpi;
            }
            break;

        //case 'f': /* file with the elliptic curve data */
        //    filename = optarg;

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
        //pollard_algorithm = pollardrho_parallel_fork;
    }

    if(pollard_algorithm != pollardrho_parallel_mpi) {

        wtc_open(0, "singlenode");
        wtc_init();
        wtc_set_name(0, "init_branches");
        wtc_set_name(1, "random number");
        wtc_set_name(2, "partition_function");
        wtc_set_name(3, "iteration_function");
        wtc_set_name(4, "calculate_result");
        wtc_set_name(5, "Modular Inverse");
        wtc_set_name(6, "other");
    }



    /*------------------------------ setting Curves and ECDLP Points -----------------------------*/
    /*
    BigInt p(229), a(1), b(44), order(239);
    BigInt Px(5), Py(116);
    BigInt Qx(155), Qy(166);

    BigInt p(69234577397554139), a(64326), b(11751), order(69234577237507391);
    BigInt Px(39361571180675947), Py(7991682211253487);
    BigInt Qx(51992249945632156), Qy(48952372232107871);


    BigInt p(7919), a(1001), b(75), order(7889);
    BigInt Px(4023), Py(6036);
    BigInt Qx(4135), Qy(3169);

    BigInt p(13576185693893), a(52852), b(48173), order(13576186067033);
    BigInt Px(4045358028067), Py(11828535504716);
    BigInt Qx(7671636463977), Qy(895724536264);
    */
    BigInt p("2879867477"), a("62293"), b("47905"), order("2879882063");
    BigInt Px(1482193291), Py(1063050205);
    BigInt Qx(2146105060), Qy(1451020666);

    init_random_number_generator(0, order);


    EllipticCurve ec(p, a, b, order);
    //ecc_description(ec);
    Point P(Px, Py);
    Point Q(Qx, Qy);
    /*-------------------------------- Curves and Points ending setting ---------------------------*/

    BigInt result = 0;

    result = (*pollard_algorithm)(ec, P, Q, iteration_function);

    cout << "Result is " << result << endl;

    if(pollard_algorithm != pollardrho_parallel_mpi) {
        wtc_print_watches();
        wtc_close();
    }

    return 0;
}
