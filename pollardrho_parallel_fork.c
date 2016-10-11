#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pollardrho.h"


#define FIFO "pfifo"


void client_func(const EllipticCurve ec,
                 const Point* P,
                 const Point* Q,
                 Triple* branches,
                 void (*iteration)(const EllipticCurve ec,
                                   mpz_t c,
                                   mpz_t d,
                                   Point* X,
                                   const Triple* branches,
                                   const unsigned long j))
{
    int ffd, i;
    init_branches(branches, ec, P, Q);

    /* BEGIN - parallel */
    Point *X = point_alloc();
    mpz_t c, d; 
    mpz_init(c);
    mpz_init(d);

    random_number(c, ec.order);
    random_number(d, ec.order);

    Point* Ptemp = point_alloc(); /* cP */
    ecc_mul(Ptemp, ec, c, P);
    Point* Qtemp = point_alloc(); /* dQ */
    ecc_mul(Qtemp, ec, d, Q);
    ecc_add(X, ec, Ptemp, Qtemp); /* X1 = cP + dQ */

    /* OBS: danger to open fifo here */
    if( (ffd = open(FIFO, O_WRONLY)) == -1 ) {
        fprintf(stderr, "Error: cannot open FIFO for writing\n");
        exit(1);
    }
    printf("Client: FIFO opened\n");

    for (i = 0; i < 100; i++) {
        int j = partition_function(X);
        (*iteration)(ec, c, d, X, branches, j);

        if ( mpz_cmp_ui(X->x, 100) < 0 ) {
            printf("Client write to fifo:");
            gmp_printf("(%Zd, %Zd, (%Zd, %Zd))\n", c, d, X->x, X->y);
            Triple t;
            mpz_init_set(t.c, c);
            mpz_init_set(t.d, d);
            t.point = *X;
            write(ffd, &t, sizeof(Triple));
        } else {
            printf("Client: Point not written\n");
        }
    }

    printf("Client: closing FIFO\n");
    close(ffd);
    /* END - parallel */

    mpz_clear(c);
    point_destroy(Ptemp);
    point_destroy(Qtemp);
    point_destroy(X);
}

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
    int i;
    Triple branches[L];

    if (mkfifo(FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 
            && errno != EEXIST) {
        fprintf(stderr, "Error: Cannot create FIFO\n");
        exit(1);
    }

    switch( fork() ) {
        case -1:
            fprintf(stderr, "Error: fork\n");
            exit(1);

        case 0:
            client_func(ec, P, Q, branches, iteration);
            break;

        default:
            ;
            int ffd;
            Triple t; 

            if( (ffd = open(FIFO, O_RDONLY)) == -1 ) {
                fprintf(stderr, "Error: cannot open FIFO for writing\n");
                exit(1);
            }
            printf("Server: FIFO opened\n");

            while( read(ffd, &t, sizeof(Triple)) > 0 ) {
                printf("SERVER READ SOMETHIN\n");
                gmp_printf("READ (%Zd, %Zd, (%Zd, %Zd))\n", t.c, t.d,
                        t.point.x, t.point.y);
            }
            close(ffd);
            printf("Server: FIFO closed\n");
    }

    //calculate_result(result, c1, c2, d1, d2, ec.order);
    
    return 0;
}
