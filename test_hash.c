#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"


int main() {
    Hashtable* mytable;
    Triple *t1, *t2, *t3, *t4;
    Point *p1, *p2, *p3;
    mpz_t m1, m2, m3, m4;

    p1 = point_alloc();
    p2 = point_alloc();
    p3 = point_alloc();
    point_init(p1, "19", "29");
    point_init(p2, "17", "26");
    point_init(p3, "19", "27");

    mpz_init_set_ui(m1, 1);
    mpz_init_set_ui(m2, 2);
    mpz_init_set_ui(m3, 3);
    mpz_init_set_ui(m4, 4);

    t1 = triple_create(m1, m2, *p1);
    t2 = triple_create(m3, m2, *p2);
    t3 = triple_create(m4, m2, *p3);

    mytable = hashtable_create(50);
    assert(mytable);

    hashtable_insert(mytable, t1);
    hashtable_insert(mytable, t2);

    t4 = NULL;
    hashtable_collide(mytable, p3, t4);
    assert(t4);
    //gmp_printf("t4(a: %Zd, b: %Zd)\n", *t4->a, *t4->b);
    /*
    assert(t4->point.x == 19 && t4->point.y == 29);

    hashtable_insert(mytable, t3);
    assert(hashtable_n_elems(mytable) == 3);
    */

    assert(hashtable_search(mytable, t1));
    return 0;
}
