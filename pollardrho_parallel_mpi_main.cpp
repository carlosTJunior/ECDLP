#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <time.h>

#include "pollardrho.h"
#include "random.h"

#define HASHTABLE_SIZE 50000
#define STRLEN 200


using namespace std;

int main(int argc, char** argv) {

    wtc_init();
    wtc_set_name(0, "init_branches");
    wtc_set_name(1, "random number");
    wtc_set_name(2, "partition_function");
    wtc_set_name(3, "iteration_function");
    wtc_set_name(4, "calculate_result");
    wtc_set_name(5, "Modular Inverse");
    wtc_set_name(6, "other");

    MPI_Init(&argc, &argv);

    /*
     * Read argv and create ECDLP here.
     * mpiexec -n 4 ./mpiprog ec.p ec.a ec.b ec.order P.x P.y Q.x Q.y
     */
    EllipticCurve ec;
    Point P;
    P.x = argv[5];
    P.y = argv[6];
    Point Q;
    Q.x = argv[7];
    Q.y = argv[8];
    Triple branches[L];

    if(argc < 9) exit(1);

    ec.p = argv[1];
    ec.a = argv[2];
    ec.b = argv[3];
    ec.order = argv[4];
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    wtc_open(rank, "MPI");

    init_random_number_generator(rank, ec.order);

    wtc_change_watch(0);
    init_branches(branches, ec, P, Q);
    wtc_change_watch(6);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* SERVER CODE */
    if (rank == 0) {
        int i;
        BigInt result = 0;

        Triple t, ct; 
        Hashtable htable(HASHTABLE_SIZE);
        int stop = 1;


        while(1) {
            char str[STRLEN] = {0};
            char* token;

            MPI_Recv(str, STRLEN, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            token = strtok(str, ":");
            t.c = token;
            token = strtok(NULL, ":");
            t.d = token;
            token = strtok(NULL, ":");
            t.point.x = token;
            token = strtok(NULL, ":");
            t.point.y = token;

            if( !htable.insert(t, ct) ) {
                /* Send stop to other processes */
                stop = 1;
                for(i = 1; i < size; i++) {
                    MPI_Send(&stop, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                }
                break;
            }
        }

        wtc_change_watch(4);
        result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);
        wtc_change_watch(6);

        printf("****** SERVER: RESULT IS %s ********\n", STR(result));

    /* CLIENTS CODE */
    } else {
        BigInt c, d;
        int stop = 0, flags = 0;

        MPI_Request recv_req;
        MPI_Status status;

        wtc_change_watch(1);
        c = random_number(ec.order) * rank % ec.order;
        d = random_number(ec.order) * rank % ec.order;
        wtc_change_watch(6);

        Point X, Ptemp, Qtemp;
        ecc_mul(Ptemp, ec, c, P);
        ecc_mul(Qtemp, ec, d, Q);
        ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

        char str[STRLEN] = {0};
        int j = -1;

        MPI_Irecv(&stop, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &recv_req);

        for (;;) {
            MPI_Request_get_status(recv_req, &flags, &status);
            if(flags){
                if(status.MPI_TAG == 1)
                    break;
            }

            wtc_change_watch(2);
            j = partition_function(X);

            //(*iteration)(ec, &c, &d, X, branches, j);
            wtc_change_watch(3);
            r_adding_walk(ec, c, d, X, branches, j);
            wtc_change_watch(6);

            if ( isDistinguished(X) ) {
                sprintf(str, "%s:%s:%s:%s", STR(c),
                                            STR(d),
                                            STR(X.x),
                                            STR(X.y));

                // Send Triple to Server
                MPI_Send(str, STRLEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                memset(str, 0, STRLEN);
            }
        }
    }
    wtc_print_watches();
    wtc_close();

    MPI_Finalize();
    return 0;
}
