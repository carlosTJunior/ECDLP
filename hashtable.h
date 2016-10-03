#ifndef _HASHTABLE
#define _HASHTABLE

#include "point.h"

typedef struct hashtable Hashtable;

typedef struct triple {
    mpz_t a;
    mpz_t b;
    Point point;
} Triple;

/* creation functions */
extern Triple* triple_create(mpz_t a, mpz_t b, Point point);
extern Hashtable* hashtable_create(long size);

/* Insert a triple into the hashtable. 
 * If triple already exists into the table, it will not be inserted.
 * Instead, triple and c_triple will store the collided triples
 * REMEMBER: To have a collision, triple->point must be the same,
 * even if the triple->a and triple->b don't collide.
 */
extern int hashtable_insert(Hashtable* hashtable, 
                            Triple* triple,
                            Triple* c_triple);

/* As Hashtable is a opaque type, it's properties cannot be accessed from
 * outside the hashtable.c file. So, it's necessary to create functions to
 * access this properties */
extern long hashtable_size(Hashtable* hashtable);
extern long hashtable_n_elems(Hashtable* hashtable);

#endif
