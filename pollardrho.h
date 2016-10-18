#ifndef _POLLARD_RHO
#define _POLLARD_RHO

#include "ecc.h"
#include "hashtable.h"

/* number of branches */
#define L 32

/* Global state to generate random numbers */
//extern gmp_randstate_t state;

/* General Pollard Rho functions */
extern int init_branches(Triple *branches, 
                         const EllipticCurve ec,
                         const Point* P,
                         const Point* Q);

extern BigInt partition_function(const Point* P);

extern BigInt random_number(const BigInt max);

extern BigInt calculate_result(const BigInt c1, 
                               const BigInt c2,
                               const BigInt d1, 
                               const BigInt d2, 
                               const BigInt order);

/* iteration functions */
extern void r_adding_walk(const EllipticCurve ec,
                          BigInt* c,
                          BigInt* d, 
                          Point* X, 
                          const Triple* branches, 
                          const unsigned long j);


/* Pollard Rho algorithms */
extern BigInt pollardrho_serial(const EllipticCurve ec, 
                                const Point* P,
                                const Point* Q, 
                                void (*iteration)(const EllipticCurve ec,
                                                  BigInt* c,
                                                  BigInt* d,
                                                  Point* X,
                                                  const Triple* branches,
                                                  const unsigned long i));

extern BigInt pollardrho_parallel_fork(const EllipticCurve ec,
                                       const Point* P,
                                       const Point* Q,
                                       void (*iteration)(const EllipticCurve ec,
                                                         BigInt* c,
                                                         BigInt* d,
                                                         Point* X,
                                                         const Triple* branches,
                                                         const unsigned long i));

extern BigInt count_0bits(BigInt x);

#endif
