#ifndef _POLLARD_RHO
#define _POLLARD_RHO

#include "ecc.h"

typedef struct _triple {
    Point point;
    mpz_t c;
    mpz_t d;
} Triple;

extern gmp_randstate_t state;

extern int pollardrho_serial(mpz_t, const EllipticCurve, const Point, \
       const Point);
extern int pollardrho_serial_openmp(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_fork(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_socket(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_openmp(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_pthread(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_mpi(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_cuda(mpz_t, EllipticCurve, Point, Point);
extern int pollardrho_parallel_opencl(mpz_t, EllipticCurve, Point, Point);

#endif
