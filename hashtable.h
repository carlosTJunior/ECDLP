#ifndef _HASHTABLE
#define _HASHTABLE

#include "point.h"

typedef struct hashtable Hashtable;
typedef struct triple {
    mpz_t a;
    mpz_t b;
    Point point;
} Triple;

extern Triple* triple_create(mpz_t a, mpz_t b, Point point);

extern Hashtable* hashtable_create(long size);
extern int hashtable_insert(Hashtable* hashtable, Triple* triple);
extern int hashtable_search(Hashtable* hashtable, Triple* triple);
extern int hashtable_collide(Hashtable* hashtable, Point* point, Triple* triple);

extern long hashtable_size(Hashtable* hashtable);
extern long hashtable_n_elems(Hashtable* hashtable);

#endif
