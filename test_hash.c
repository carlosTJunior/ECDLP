#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"

Point* point_create(int x, int y) {
    Point* p = (Point*) malloc(sizeof(Point));
    p->x = x;
    p->y = y;

    return p;
}

int main() {
    Hashtable* mytable;
    Triple *t1, *t2, *t3;
    Point *p1, *p2, *p3;
    p1 = point_create(19, 29);
    p2 = point_create(17, 26);
    p3 = point_create(19, 23);
    t1 = triple_create(1, 2, *p1);
    t2 = triple_create(3, 2, *p2);
    t3 = triple_create(4, 2, *p3);

    mytable = hashtable_create(50);
    assert(mytable);

    hashtable_insert(mytable, t1);
    hashtable_insert(mytable, t2);
    hashtable_insert(mytable, t3);
    assert(hashtable_n_elems(mytable) == 3);

    assert(hashtable_search(mytable, t1));
    return 0;
}
