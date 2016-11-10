#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include "pollardrho.h"

#define NUM_PROCESSES 4

/*
 * This function will call mpiexec to execute the mpiprog 
 *
 */

BigInt pollardrho_parallel_mpi(const EllipticCurve ec, 
                               const Point P,
                               const Point Q, 
                               void (*iteration)(const EllipticCurve ec,
                                                 BigInt& c,
                                                 BigInt& d,
                                                 Point& X,
                                                 const Triple* branches,
                                                 const unsigned long i))
{
    iteration = NULL;
    char command[250] = {0};
    BigInt res(1);
    sprintf(command, "mpiexec -n %d ./mpiprog %s %s %s %s %s %s %s %s",
                                 NUM_PROCESSES,
                                 STR(ec.p),
                                 STR(ec.a),
                                 STR(ec.b),
                                 STR(ec.order),
                                 STR(P.x),
                                 STR(P.y),
                                 STR(Q.x),
                                 STR(Q.y));

    system(command);

    return res;
}
