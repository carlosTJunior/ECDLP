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
    mpz_init_set_ui(m5, 0);

    t1 = triple_create(m1, m2, *p1);
    t2 = triple_create(m3, m2, *p2);
    t3 = triple_create(m4, m2, *p3);
    collided_triple = triple_create(m5, m5, *dummy_point);
    gmp_printf("t1 = (%Zd, %Zd, (%Zd, %Zd))\n", t1->c, t1->d, t1->point.x, t1->point.y);
    gmp_printf("t2 = (%Zd, %Zd, (%Zd, %Zd))\n", t2->c, t2->d, t2->point.x, t2->point.y);
    gmp_printf("t3 = (%Zd, %Zd, (%Zd, %Zd))\n", t3->c, t3->d, t3->point.x, t3->point.y);
    gmp_printf("ct = (%Zd, %Zd, (%Zd, %Zd))\n", 
            collided_triple->c, collided_triple->d, collided_triple->point.x,
            collided_triple->point.y);

    mytable = hashtable_create(50);
    assert(mytable);

    hashtable_insert(mytable, t1, collided_triple);
    hashtable_insert(mytable, t2, collided_triple);
    hashtable_insert(mytable, t3, collided_triple);

    //assert(hashtable_n_elems(mytable) == 3);

    //assert(hashtable_search(mytable, t1));
    return 0;
}
