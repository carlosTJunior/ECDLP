#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
//#include <sys/sysinfo.h>
#include "hashtable.h"


class List {
public:
    Triple data;
    List* next;

    List() : next(0) {}
    List(Triple t, List* n) : data(t), next(n) {}
}; 

class Chain {
public:
    List *list;

    Chain() : list(0) {}
    ~Chain() {}
    void insert(const Triple triple);
    bool search(const Triple triple, Triple& c_triple) const;
};


void Chain::insert(const Triple triple) {
    this->list = new List(triple, this->list);
}

bool Chain::search(const Triple triple, Triple& c_triple) const {
    List* temp;

    temp = this->list;
    while(temp) {
        if(temp->data.point == triple.point) {
            c_triple.c = temp->data.c;
            c_triple.d = temp->data.d;
            c_triple.point = temp->data.point;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

hashkey_t point_xor(hashkey_t *addr, int siz)
{
    const int n=siz/sizeof(hashkey_t);

    hashkey_t ret=*addr;
    for (int i=1; i<n; i++) ret = ret ^ addr[i];
    //wtc_printf("XOR %ld\n", ret);
    return ret;
}

/* Temporary hash function */
hashkey_t hash (const Triple* triple, hashkey_t size)
{
    if ((sizeof(Point)%sizeof(hashkey_t)) ||
        (sizeof(Point)<sizeof(hashkey_t)))
    {
        wtc_printf("Error: sizeof(hashkey_t) must be multiple of sizeof(Point)\n");
        exit(1);
    }
    return point_xor((hashkey_t*)&triple->point, sizeof(Point))%size;
}

Hashtable::Hashtable (const unsigned long size) {
    this->size = size;

    this->chain = (Chain*) malloc(size * sizeof(Chain));

    for(unsigned long i = 0; i < size; i++) {
        this->chain[i] = Chain();
    }
}

/* Function to insert into the hashtable */
bool Hashtable::insert(const Triple triple, Triple& c_triple) {
    long h;

    h = hash(&triple, this->size);

    if ( !this->chain[h].search(triple, c_triple) ) {
        //std::cout << "Triple(" << triple.c << ", " << triple.d << ",(" << triple.point.x << ", " << triple.point.y << ")) Inserted\n";
        //wtc_printf("Inserting (%lld, %lld, (%lld, %lld)) into the hashtable position %lld\n",
        //        triple->c, triple->d, triple->point.x, triple->point.y, h);
        this->chain[h].insert(triple);
        this->n_elems++;
    } else {
        std::cout << "Collision found in point (";
        std::cout << triple.point.x << ", " << triple.point.y << ")\n";
        std::cout << "\ta1 = " << triple.c << ", b1 = " << triple.d << "\n";
        std::cout << "\ta2 = " << c_triple.c << ", b2 = " << c_triple.d << "\n";
        return false;
    }
    return true;
}
