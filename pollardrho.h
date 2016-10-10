#ifndef _POLLARD_RHO
#define _POLLARD_RHO

#include "ecc.h"
#include "hashtable.h"

/* number of branches */
#define L 32

/* Global state to generate random numbers */
extern gmp_randstate_t state;

/* General Pollard Rho functions */
extern int init_branches(Triple *branches, 
                         const EllipticCurve ec,
                         const Point* P,
                         const Point* Q);

extern unsigned long int partition_function(const Point* P);

extern void random_number(mpz_t result, const mpz_t max);

extern int calculate_result(mpz_t result, 
                            const mpz_t c1, 
                            const mpz_t c2,
                            const mpz_t d1, 
                            const mpz_t d2, 
                            const mpz_t order);

/* iteration functions */
extern void r_walking(const EllipticCurve ec,
                      mpz_t c,
                      mpz_t d, 
                      Point* X, 
                      const Triple* branches, 
                      const unsigned long j);


/* Pollard Rho algorithms */
extern int pollardrho_serial(mpz_t result, 
                             const EllipticCurve ec, 
                             const Point* P,
                             const Point* Q, 
                             void (*iteration)(const EllipticCurve ec,
                                               mpz_t c,
                                               mpz_t d,
                                               Point* X,
                                               const Triple* branches,
                                               const unsigned long i));

extern int pollardrho_parallel_fork(mpz_t, const EllipticCurve, const Point*, \
       const Point*);
#endif
