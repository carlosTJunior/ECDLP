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
                                 ec.p.get_str(10).c_str(),
                                 ec.a.get_str(10).c_str(),
                                 ec.b.get_str(10).c_str(),
                                 ec.order.get_str(10).c_str(),
                                 P.x.get_str(10).c_str(),
                                 P.y.get_str(10).c_str(),
                                 Q.x.get_str(10).c_str(),
                                 Q.y.get_str(10).c_str());

    system(command);

    return res;
}
