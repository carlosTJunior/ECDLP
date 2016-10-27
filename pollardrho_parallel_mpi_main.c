#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* function offsetof() */
#include <mpi.h>
#include <time.h>

#include "pollardrho.h"

#define HASHTABLE_SIZE 50000

void client(int rank);
void server(int rank, int size);

int main(int argc, char** argv) {
    srandom(time(NULL));
    MPI_Init(&argc, &argv);

    /*
     * Read argv and create ECDLP here.
     * mpiexec -n 4 ./mpiprog ec.p ec.a ec.b ec.order P.x P.y Q.x Q.y
     */
    EllipticCurve ec;
    Point *P, *Q;
    Triple* branches;

    if(argc < 9) exit(1);

    ec.p = atol(argv[1]);
    ec.a = atol(argv[2]);
    ec.b = atol(argv[3]);
    ec.order = atol(argv[4]);

    P = point_alloc();
    Q = point_alloc();

    point_init(P, atol(argv[5]), atol(argv[6]));
    point_init(Q, atol(argv[7]), atol(argv[8]));

    int rank, size;

    branches = (Triple*) malloc(32 * sizeof(Triple));

    init_branches(branches, ec, P, Q);


    /* 
     * Create MPI_Triple_type for our Triple here
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
    offsets[2] = offsetof(Triple, point.x);
    offsets[3] = offsetof(Triple, point.y);
    MPI_Type_create_struct(nItems, 
                           blocklengths, 
                           offsets, 
                           types, 
                           &MPI_Triple_type);
    MPI_Type_commit(&MPI_Triple_type);


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* SERVER CODE */
    if (rank == 0) {
        //server(rank, size);
        int i;
        Triple branches[L];
        BigInt result = 0;

        int numRead;
        Triple t, ct; 
        Hashtable* htable;
        htable = hashtable_create(HASHTABLE_SIZE);

        while(1) {
            MPI_Recv(&t, 1, MPI_Triple_type, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if( !hashtable_insert(htable, &t, &ct) ) {
                int stop = 1;
                /* Send stop to other processes */
                for(i = 1; i < size; i++) {
                    MPI_Send(&stop, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                break;
            }
        }

        result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);

        printf("****** SERVER: RESULT IS %lld ********\n", result);

    /* CLIENTS CODE */
    } else {
        //client(rank);
        BigInt c, d;
        int stop = 0;
        MPI_Request recv_req;

        Point *X = point_alloc();

        c = random_number(ec.order) * rank % ec.order;
        d = random_number(ec.order) * rank % ec.order;

        Point* Ptemp = point_alloc(); /* cP */
        ecc_mul(Ptemp, ec, c, P);
        Point* Qtemp = point_alloc(); /* dQ */
        ecc_mul(Qtemp, ec, d, Q);
        ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

        for (;;) {
            MPI_Irecv(&stop, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_req);
            if(stop)    break;

            int j = partition_function(X);

            //(*iteration)(ec, &c, &d, X, branches, j);
            r_adding_walk(ec, &c, &d, X, branches, j);

            if ( isDistinguished(X) ) {
                Triple t;
                t.c = c;
                t.d = d;
                point_copy(&t.point, X);

                // Send Triple to Server
                MPI_Send(&t, 1, MPI_Triple_type, 0, 0, MPI_COMM_WORLD);
            }
        }
        //printf("----- Client %d finishing execution ------\n", rank);

        point_destroy(Ptemp);
        point_destroy(Qtemp);
        point_destroy(X);
    }

    MPI_Type_free(&MPI_Triple_type);
    MPI_Finalize();
}
