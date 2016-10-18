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
#define HASHTABLE_SIZE 50

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
    int ffd, i;
    BigInt c, d;
    init_branches(branches, ec, P, Q);

    /* BEGIN - parallel */
    Point *X = point_alloc();

    c = random_number(ec.order);
    d = random_number(ec.order);

    Point* Ptemp = point_alloc(); /* cP */
    ecc_mul(Ptemp, ec, c, P);
    Point* Qtemp = point_alloc(); /* dQ */
    ecc_mul(Qtemp, ec, d, Q);
    ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

    /* OBS: danger to open fifo here (parallel zone) */
    if( (ffd = open(FIFO, O_WRONLY)) == -1 ) {
        fprintf(stderr, "Error: cannot open FIFO for writing\n");
        exit(1);
    }
    printf("Client: FIFO opened\n");

    for (;;) {
        int j = partition_function(X);
        (*iteration)(ec, &c, &d, X, branches, j);

        //if ( X->x != -1 && count_0bits(X->x) > 40) { /* change condition to distinguished point function */
        if ( X->x != -1 && X->x < 50) { /* change condition to distinguished point function */
            //printf("Client write to fifo:");
            //printf("(%ld, %ld, (%ld, %ld))\n", c, d, X->x, X->y);
            Triple t;
            t.c = c;
            t.d = d;
            t.point = *X;
            write(ffd, &t, sizeof(Triple));
        }
    }

    printf("Client: closing FIFO\n");
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
    int i;
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

        case 0:
            client_func(ec, P, Q, branches, iteration);
            break;

        default:
            ;
            int ffd;
            Triple t, ct; 
            Hashtable* htable;
            htable = hashtable_create(HASHTABLE_SIZE);

            if( (ffd = open(FIFO, O_RDONLY)) == -1 ) {
                fprintf(stderr, "Error: cannot open FIFO for writing\n");
                exit(1);
            }
            printf("Server: FIFO opened\n");

            while( read(ffd, &t, sizeof(Triple)) > 0 ) {
                //printf("SERVER READ: ");
                //printf("(%ld, %ld, (%ld, %ld))\n", t.c, t.d,
                //        t.point.x, t.point.y);
                if( !hashtable_insert(htable, &t, &ct) ) {
                    /* Kill child process before return */
                    kill(chldPid, SIGTERM);
                    break;
                }
            }
            close(ffd);
            printf("Server: FIFO closed\n");
            result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);
    }

    return result;
}
