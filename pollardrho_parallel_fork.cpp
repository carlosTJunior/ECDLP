#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#include "pollardrho.h"

#define FIFO "pfifo"
#define HASHTABLE_SIZE 50000
#define STRLEN 200

using namespace std;

void client_func(const EllipticCurve ec,
                 const Point P,
                 const Point Q,
                 Triple* branches,
                 void (*iteration)(const EllipticCurve ec,
                                   BigInt& c,
                                   BigInt& d,
                                   Point& X,
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
    cout << "Client opened FIFO\n";

    /* BEGIN - parallel */
    Point X;

    c = random_number(ec.order);
    d = random_number(ec.order);

    Point Ptemp, Qtemp;
    ecc_mul(Ptemp, ec, c, P);
    ecc_mul(Qtemp, ec, d, Q);
    ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

    for (;;) {
        cout << "C";
        int j = partition_function(X);
        (*iteration)(ec, c, d, X, branches, j);

        if ( isDistinguished(X) ) {
            char *str;

            sprintf(str, "%s:%s:%s:%s", c.get_str(10).c_str(),
                                        d.get_str(10).c_str(),
                                        X.x.get_str(10).c_str(),
                                        X.y.get_str(10).c_str());

            if(write(ffd, str, strlen(str) ) > 0) {
                printf("CLIENT WROTE %s\n", str);
            }
        }
    }

    close(ffd);
    /* END - parallel */
}

BigInt pollardrho_parallel_fork(const EllipticCurve ec,
                                const Point P,
                                const Point Q,
                                void (*iteration)(const EllipticCurve ec,
                                                  BigInt& c,
                                                  BigInt& d,
                                                  Point& X,
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
            Hashtable htable(HASHTABLE_SIZE);

            if( (ffd = open(FIFO, O_RDONLY)) == -1 ) {
                fprintf(stderr, "Error: cannot open FIFO for writing\n");
                exit(1);
            }
            cout << "Server opened FIFO\n";

            /* Modify flags to use nonblocking read on fifo */
            int flags = fcntl(ffd, F_GETFL);
            flags |= O_NONBLOCK;
            if(fcntl(ffd, F_SETFL, flags) == -1)
                fprintf(stderr, "Cannot modify flags\n");

            char str[STRLEN];
            memset(str, 0, STRLEN); 
            char *token;
            while(1) {
                numRead = read(ffd, str, STRLEN);
                if( numRead > 0 ) {
                    token = strtok(str, ":");
                    t.c = token;
                    token = strtok(NULL, ":");
                    t.d = token;
                    token = strtok(NULL, ":");
                    t.point.x = token;
                    token = strtok(NULL, ":");
                    t.point.y = token;
                    cout << "READ (" << t.c << ", " << t.d << ", " << t.point.x << ", " << t.point.y << ")\n";
                    if( !htable.insert(t, ct) ) {
                        /* Kill child process before return */
                        kill(chldPid, SIGTERM);
                        break;
                    }
                }
                memset(str, 0, STRLEN); 
            }
            close(ffd);
            result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);
    }

    return result;
}
