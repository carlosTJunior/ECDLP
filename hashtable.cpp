#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
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

long hash (const Triple triple, unsigned long size) {

    return (triple.point.x).get_si() % size;
    //return triple.point.x % size;
}


Hashtable::Hashtable (long size) {
    this->size = size;

    this->chain = (Chain*) malloc(size * sizeof(Chain));

    for(int i = 0; i < size; i++) {
        this->chain[i] = Chain();
    }
}

/* Function to insert into the hashtable */
bool Hashtable::insert(const Triple triple, Triple& c_triple) {
    long h;

    h = hash(triple, this->size);

    if ( !this->chain[h].search(triple, c_triple) ) {
        //std::cout << "Triple(" << triple.c << ", " << triple.d << ",(" << triple.point.x << ", " << triple.point.y << ")) Inserted\n";
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
