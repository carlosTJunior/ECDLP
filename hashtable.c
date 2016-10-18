#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"


Triple* triple_create(const BigInt c, const BigInt d, const Point point) {
    Triple* t = (Triple*) malloc(sizeof(Triple));

    t->c = c;
    t->d = d;
    t->point = point;

    return t;
}

Triple* triple_copy(const Triple* triple) {
    Triple* t = (Triple*) malloc(sizeof(Triple));

    t->c = triple->c;
    t->d = triple->d;
    t->point = triple->point;

    return t;
}

typedef struct list {
    Triple* data;
    struct list* next;
} List;

typedef struct chain {
    List *list;
} Chain;


int chain_insert(Chain* chain, const Triple* triple) {
    List *new_node;
    if(!chain || !triple) return -1;

    new_node = (List*) malloc(sizeof(List));
    new_node->data = triple_copy(triple);
    new_node->next = chain->list;

    chain->list = new_node;
    return 0;
}

int chain_search(const Chain* chain, const Triple* triple, Triple* c_triple) {
    List* temp;
    if (!chain || !triple) return -1;

    temp = chain->list;
    while(temp) {
    /* Change this. It should use a hash obtained from point->x and point->y values */
        if(point_is_equal(&temp->data->point, &triple->point)) {
            c_triple->c = temp->data->c;
            c_triple->d = temp->data->d;
            c_triple->point.x = temp->data->point.x;
            c_triple->point.y = temp->data->point.y;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}


struct hashtable {
    long size;
    long n_elems;
    Chain* chain;
};

/* Temporary hash function */
long hash (const Triple* triple, unsigned long size) {

    return triple->point.x % size;
}

Hashtable* hashtable_create(const long size) {
    Hashtable* table = (Hashtable*) malloc(sizeof(Hashtable));
    table->size = size;
    table->n_elems = 0;
    table->chain = (Chain*) calloc(size, sizeof(Chain));

    return table;
}

long hashtable_size(const Hashtable* hashtable) {
    return hashtable->size;
}

long hashtable_n_elems(const Hashtable* hashtable) {
    return hashtable->n_elems;
}

/* Function to insert into the hashtable, returns the hashtable's n_elems or -1 on error */
int hashtable_insert(Hashtable* hashtable, const Triple* triple, Triple* c_triple) {
    long h;
    if(!hashtable || !triple || !c_triple) return -1;

    h = hash(triple, hashtable->size);
    if (!chain_search(&hashtable->chain[h], triple, c_triple)) {
        //printf("Inserting (%ld, %ld, (%ld, %ld)) into the hashtable position %ld\n",
        //        triple->c, triple->d, triple->point.x, triple->point.y, h);
        chain_insert(&hashtable->chain[h], triple);
        hashtable->n_elems++;
    } else {
        printf("Collision found in point (%ld, %ld)\n",
                     triple->point.x, triple->point.y);
        printf("\ta1 = %ld, b1 = %ld\n", triple->c, triple->d);
        printf("\ta2 = %ld, b2 = %ld\n", c_triple->c, c_triple->d);
        /* return FALSE = 0 if cannot insert */
        return 0;
    }
    /* return TRUE = 1 if inserted with success */
    return 1;
}
