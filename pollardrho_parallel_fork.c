#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

int pollardrho_parallel_fork(mpz_t result,
                             const EllipticCurve ec,
                             const Point* P,
                             const Point* Q,
                             void (*iteration)(const EllipticCurve ec,
                                               mpz_t c,
                                               mpz_t d,
                                               Point* X,
                                               const Triple* branches,
                                               const unsigned long i))
{
    Triple branches[L];
    int i;
    int pipefd[2];

    init_branches(branches, ec, P, Q);
    if( pipe(pipefd) == -1 ) {
        fprintf(stderr, "Error: pipe\n");
        exit(1);
    }

    switch( fork() ) {
        case -1:
            fprintf(stderr, "Error: fork\n");
            exit(1);

        case 0:
            calculate_points(pipefd);
            break;

        default:
            server(pipefd);
            break;
    }

    calculate_result(result, c1, c2, d1, d2, ec.order);
    
    return 0;
}
