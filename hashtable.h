#ifndef _HASHTABLE
#define _HASHTABLE

#include "point.h"
#include "watches.h"

#define hashkey_t unsigned long

struct Triple {
    BigInt c;
    BigInt d;
    Point point;

    Triple() {}
    ~Triple() {}
    Triple(BigInt c, BigInt d, Point P) : c(c), d(d), point(P) {}
};

class Chain;

struct Hashtable {
    long size;
    BigInt n_elems;
    Chain* chain;

    Hashtable (const unsigned long size);
    /*
     * If insert return false, then it found a collision.
     * The c_triple reference will store the value of the triple
     * which have a collision.
     */
    bool insert(const Triple triple,
                Triple& c_triple);
};

#endif
