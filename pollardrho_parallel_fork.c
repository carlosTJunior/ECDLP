#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#include "pollardrho.h"


#define FIFO "pfifo"
#define HASHTABLE_SIZE 50000


void client_func(const EllipticCurve ec,
                 const Point* P,
                 const Point* Q,
                 Triple* branches,
                 void (*iteration)(const EllipticCurve ec,
                                   BigInt* c,
                                   BigInt* d,
                                   Point* X,
                                   const Triple* branches,
                                   const unsigned long j))
{
    int ffd;
    BigInt c, d;
    init_branches(branches, ec, P, Q);

    /* Open FIFO to process - will be shared by threads */
    if( (ffd = open(FIFO, O_WRONLY)) == -1 ) {
        fprintf(stderr, "Error: cannot open FIFO for writing\n");
        exit(1);
    }

    /* BEGIN - parallel */
    Point *X = point_alloc();

    c = random_number(ec.order);
    d = random_number(ec.order);

    Point* Ptemp = point_alloc(); /* cP */
    ecc_mul(Ptemp, ec, c, P);
    Point* Qtemp = point_alloc(); /* dQ */
    ecc_mul(Qtemp, ec, d, Q);
    ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

    for (;;) {
        int j = partition_function(X);
        (*iteration)(ec, &c, &d, X, branches, j);

        if ( isDistinguished(X) ) {
            Triple t;
            t.c = c;
            t.d = d;
            point_copy(&t.point, X);
            if(write(ffd, &t, 4 * sizeof(BigInt)) > 0) {
            }
        }
    }

    close(ffd);
    /* END - parallel */

    point_destroy(Ptemp);
    point_destroy(Qtemp);
    point_destroy(X);
}

BigInt pollardrho_parallel_fork(const EllipticCurve ec,
                                const Point* P,
                                const Point* Q,
                                void (*iteration)(const EllipticCurve ec,
                                                  BigInt* c,
                                                  BigInt* d,
                                                  Point* X,
                                                  const Triple* branches,
                                                  const unsigned long i))
{
    Triple branches[L];
    BigInt result = 0;
    int chldPid;

    if (mkfifo(FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 
            && errno != EEXIST) {
        fprintf(stderr, "Error: Cannot create FIFO\n");
        exit(1);
    }

    switch( chldPid = fork() ) {
        case -1:
            fprintf(stderr, "Error: fork\n");
            exit(1);

        case 0: /* Client which generates points */
            client_func(ec, P, Q, branches, iteration);
            break;

        default: /* Server which receive points */
            ;
            int ffd, numRead;
            Triple t, ct; 
            Hashtable* htable;
            htable = hashtable_create(HASHTABLE_SIZE);

            if( (ffd = open(FIFO, O_RDONLY)) == -1 ) {
                fprintf(stderr, "Error: cannot open FIFO for writing\n");
                exit(1);
            }

            /* Modify flags to use nonblocking read on fifo */
            int flags = fcntl(ffd, F_GETFL);
            flags |= O_NONBLOCK;
            if(fcntl(ffd, F_SETFL, flags) == -1)
                fprintf(stderr, "Cannot modify flags\n");

            while(1) {
                numRead = read(ffd, &t, 4 * sizeof(BigInt));
                if( numRead > 0) {
                    if( !hashtable_insert(htable, &t, &ct) ) {
                        /* Kill child process before return */
                        kill(chldPid, SIGTERM);
                        break;
                    }
                }
            }
            close(ffd);
            result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);
    }

    return result;
}
