#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* function offsetof() */
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <mpi.h>

#include "pollardrho.h"

#define HASHTABLE_SIZE 50

void client();
void server();

int main(int argc, char** argv) {
    /*
     * Read argv and create ECDLP here
     */

    int rank, size;

    init_branches(branches, ec, P, Q);

    MPI_Init();

    /* 
     * Create MPI_Triple_type here
     * For now: with 4 long longs instead of
     * 2 long longs for c and d and one pointer
     */
    const int nItems = 4;
    int blocklengths[4] = {1, 1, 1, 1};
    MPI_Datatype types[4] = {MPI_LONG_LONG_INT,
                             MPI_LONG_LONG_INT,
                             MPI_LONG_LONG_INT,
                             MPI_LONG_LONG_INT};
    MPI_Datatype MPI_Triple_type;
    MPI_Aint offsets[4];
    offsets[0] = offsetof(Triple, c);
    offsets[1] = offsetof(Triple, d);
    offsets[2] = offsetof(Triple, Point.x);
    offsets[3] = offsetof(Triple, Point.y);
    MPI_Type_create_struct(nItems, 
                           blocklengths, 
                           offsets, 
                           types, 
                           &MPI_Triple_type);
    MPI_Type_commit(&MPI_Triple_type);


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        server();
    } else {
        client();
    }

    MPI_Type_free(&MPI_Triple_type);
    MPI_Finalize();
}

void client()
{
    BigInt c, d;

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
        printf("Client running (%lld, %lld)\n", X->x, X->y);

        if ( X->x != -1 && X->x < 200) {
            Triple t;
            t.c = c;
            t.d = d;
            point_copy(&t.point, X);

            // Send Triple to Server
        }
    }

    point_destroy(Ptemp);
    point_destroy(Qtemp);
    point_destroy(X);
}

void server()
{
    int i;
    Triple branches[L];
    BigInt result = 0;

    int numRead;
    Triple t, ct; 
    Hashtable* htable;
    htable = hashtable_create(HASHTABLE_SIZE);

    while(1) {
        printf("Server running/ ");
        numRead = read(ffd, &t, 4 * sizeof(BigInt));
        if( numRead > 0) {
            printf("SERVER READ: ");
            if( !hashtable_insert(htable, &t, &ct) ) {
                /* Kill child process before return */
                kill(chldPid, SIGTERM);
                break;
            }
        }
    }

    result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);

    //return result;
}
