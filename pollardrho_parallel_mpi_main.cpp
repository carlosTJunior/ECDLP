#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stddef.h> /* function offsetof() */
#include <mpi.h>
#include <time.h>

#include "pollardrho.h"

#define HASHTABLE_SIZE 50000
#define STRLEN 200

void client(int rank);
void server(int rank, int size);

using namespace std;

int main(int argc, char** argv) {
    srandom(time(NULL));
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
    Triple branches[32];

    if(argc < 9) exit(1);

    ec.p = argv[1];
    ec.a = argv[2];
    ec.b = argv[3];
    ec.order = argv[4];

    int rank, size;

    init_branches(branches, ec, P, Q);

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
        Hashtable htable(HASHTABLE_SIZE);

        char str[STRLEN];
        memset(str, 0, STRLEN);
        char* token;

        while(1) {
            //cout << "Server here\n"; 
            MPI_Recv(str, STRLEN, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "RECEIVED: " << str << "\n";

            token = strtok(str, ":");
            t.c = token;
            token = strtok(NULL, ":");
            t.d = token;
            token = strtok(NULL, ":");
            t.point.x = token;
            token = strtok(NULL, ":");
            t.point.y = token;

            if( !htable.insert(t, ct) ) {
                //cout << "Cannot insert into hashtable\n";
                int stop = 1;
                /* Send stop to other processes */
                for(i = 1; i < size; i++) {
                    MPI_Send(&stop, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                break;
            }
            memset(str, 0, STRLEN);
        }

        result = calculate_result(t.c, ct.c, t.d, ct.d, ec.order);

        printf("****** SERVER: RESULT IS %s ********\n", result.get_str().c_str());

    /* CLIENTS CODE */
    } else {
        //client(rank);
        BigInt c, d;
        int stop = 0;
        MPI_Request recv_req;

        c = random_number(ec.order) * rank % ec.order;
        d = random_number(ec.order) * rank % ec.order;

        Point X, Ptemp, Qtemp;
        ecc_mul(Ptemp, ec, c, P);
        ecc_mul(Qtemp, ec, d, Q);
        ecc_add(X, ec, Ptemp, Qtemp); /* X = cP + dQ */

        cout << "Client " << rank << endl;

        for (;;) {
            //cout << "Client here\n";
            MPI_Irecv(&stop, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_req);
            if(stop)    break;

            int j = partition_function(X);

            //(*iteration)(ec, &c, &d, X, branches, j);
            r_adding_walk(ec, c, d, X, branches, j);

            if ( isDistinguished(X) ) {
                char *str;
                sprintf(str, "%s:%s:%s:%s", c.get_str(10).c_str(),
                                            d.get_str(10).c_str(),
                                            X.x.get_str(10).c_str(),
                                            X.y.get_str(10).c_str());
                cout << "Sending (" << str << ")\n";

                // Send Triple to Server
                MPI_Send(str, STRLEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
        //printf("----- Client %d finishing execution ------\n", rank);
    }

    MPI_Finalize();
}
