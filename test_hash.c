#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"


int main() {
    Hashtable* mytable;
    Triple *t1, *t2, *t3, *collided_triple;
    Point *p1, *p2, *p3, *dummy_point;
    mpz_t m1, m2, m3, m4, m5;

    p1 = point_alloc();
    p2 = point_alloc();
    p3 = point_alloc();
    dummy_point = point_alloc();
    point_init(p1, "19", "29");
    point_init(p2, "17", "26");
    point_init(p3, "19", "29");
    point_init(dummy_point, "-1", "-1");

    mpz_init_set_ui(m1, 1);
    mpz_init_set_ui(m2, 2);
    mpz_init_set_ui(m3, 3);
    mpz_init_set_ui(m4, 4);
    mpz_init_set_ui(m4, 0);

    t1 = triple_create(m1, m2, *p1);
    t2 = triple_create(m3, m2, *p2);
    t3 = triple_create(m4, m2, *p3);
    collided_triple = triple_create(m5, m5, *dummy_point);

    mytable = hashtable_create(50);
    assert(mytable);

    hashtable_insert(mytable, t1, collided_triple);
    hashtable_insert(mytable, t2, collided_triple);
    hashtable_insert(mytable, t3, collided_triple);

    //assert(hashtable_n_elems(mytable) == 3);

    //assert(hashtable_search(mytable, t1));
    return 0;
}
