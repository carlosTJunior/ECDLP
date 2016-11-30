#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <sys/sysinfo.h>
#include "hashtable.h"
#include "watches.h"


class List {
public:
    Triple data;
    List* next;

    List() : next(0) {}
    List(Triple t, List* n) : data(t), next(n) {}
}; 
static List* nodes;

class Chain {
public:
    List *list;

    Chain() : list(0) {}
    ~Chain() {}
    void insert(const Triple triple, long idx);
    bool search(const Triple triple, Triple& c_triple) const;
};


void Chain::insert(const Triple triple, long idx) {
    List& new_node = nodes[idx];
    //memcpy(&new_node->data, triple, sizeof(Triple));
    new_node.data = triple;

    new_node.next = this->list;
    //this->list = new List(triple, this->list);
    this->list = &new_node;
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

/*
long hash (const Triple triple, unsigned long size) {

    return (triple.point.x).get_si() % size;
    //return triple.point.x % size;
}
*/

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

Hashtable::Hashtable (long size) {

    // Obtem a quantidade de memoria do noh
    struct sysinfo info;
    const int ierr = sysinfo(&info);
    wtc_printf("info.totalram %ld MiB\n", info.totalram * info.mem_unit / 1024 / 1024);
    wtc_printf("sizeof(List) %lu\n", sizeof(List));
    // msize = total bytes used by hash nodes
    // Hardwired: 1/20 of total RAM memory
    const size_t msize = (info.totalram * (size_t)info.mem_unit) / 20;
    std::cout << "msize = " << msize << std::endl;

    this->maxnodes = msize / sizeof(List);
    this->size = this->maxnodes;
    this->n_elems = 0;

    this->chain = (Chain*) calloc(this->size, sizeof(Chain));
    nodes = (List*) calloc(this->maxnodes, sizeof(List));

}

/* Function to insert into the hashtable */
bool Hashtable::insert(const Triple triple, Triple& c_triple) {
    long h;

    h = hash(&triple, this->size);

    if ( !this->chain[h].search(triple, c_triple) ) {
        //wtc_printf("Inserting (%lld, %lld, (%lld, %lld)) into the hashtable position %lld\n",
        //        triple->c, triple->d, triple->point.x, triple->point.y, h);
        this->chain[h].insert(triple, this->n_elems);
        this->n_elems++;
        if (this->n_elems >= this->size)
        {
            wtc_printf("Error: HASH exaust.\n");
            exit(1);
        }
    } else {
        //wtc_printf("Collision found in point (%lld, %lld)\n",
        //            triple->point.x, triple->point.y);
        //wtc_printf("\ta1 = %lld, b1 = %lld\n", triple->c, triple->d);
        //            wtc_printf("\ta2 = %lld, b2 = %lld\n", c_triple->c, c_triple->d);
        std::cout << "Collision found in point (";
        std::cout << triple.point.x << ", " << triple.point.y << ")\n";
        std::cout << "\ta1 = " << triple.c << ", b1 = " << triple.d << "\n";
        std::cout << "\ta2 = " << c_triple.c << ", b2 = " << c_triple.d << "\n";
        return false;
    }
    return true;
}
