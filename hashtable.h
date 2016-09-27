#ifndef _HASHTABLE
#define _HASHTABLE

/*
#include "point.h"
*/

/* temporary struct point to test implementation */
typedef struct point {
    int x;
    int y;
} Point;

typedef struct hashtable Hashtable;
typedef struct triple Triple;

extern Triple* triple_create(long a, long b, Point point);

extern Hashtable* hashtable_create(long size);
extern int hashtable_insert(Hashtable* hashtable, Triple* triple);
extern int hashtable_search(Hashtable* hashtable, Triple* triple);

extern long hashtable_size(Hashtable* hashtable);
extern long hashtable_n_elems(Hashtable* hashtable);

#endif
