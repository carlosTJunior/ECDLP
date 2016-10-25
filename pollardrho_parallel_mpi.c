#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "pollardrho.h"

#define NUM_PROCESSES 4

BigInt pollardrho_parallel_mpi(const EllipticCurve ec, 
                               const Point* P,
                               const Point* Q, 
                               void (*iteration)(const EllipticCurve ec,
                                                 BigInt* c,
                                                 BigInt* d,
                                                 Point* X,
                                                 const Triple* branches,
                                                 const unsigned long i))
{
    char *command;
    sprintf(command, "mpiexec -n %d ./mpiprog %lld %lld %lld %lld %lld %lld %lld %lld",
                                 NUM_PROCESSES,
                                 ec.p,
                                 ec.a,
                                 ec.b,
                                 ec.order,
                                 P->x,
                                 P->y,
                                 Q->x,
                                 Q->y);

    system(command);

    return 0;
}
