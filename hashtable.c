#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"

struct triple {
    long a;
    long b;
    Point point;
};

Triple* triple_create(long a, long b, Point point) {
    Triple* t = (Triple*) malloc(sizeof(Triple));
    t->a = a;
    t->b = b;
    t->point = point;

    return t;
}

typedef struct list {
    Triple* data;
    struct list* next;
} List;

typedef struct chain {
    List *list;
} Chain;

int chain_insert(Chain* chain, Triple* triple) {
    List *new_node;
    if(!chain || !triple) return -1;

    new_node = (List*) malloc(sizeof(List));
    new_node->data = triple;
    new_node->next = chain->list;

    chain->list = new_node;
    return 0;
}

int chain_search(Chain* chain, Triple* triple) {
    List* temp;
    if (!chain || !triple) return -1;

    temp = chain->list;
    while(temp) {
        /* Change this. It should use a hash obtained from point->x and point->y values */
        if(temp->data->point.x == triple->point.x && temp->data->point.y == triple->point.y)
            return 1;
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
long hash (Triple* triple, long size) {
    return triple->point.x % size;
}

Hashtable* hashtable_create(long size) {
    Hashtable* table = (Hashtable*) malloc(sizeof(Hashtable));
    table->size = size;
    table->n_elems = 0;
    table->chain = (Chain*) calloc(size, sizeof(Chain));

    return table;
}

long hashtable_size(Hashtable* hashtable) {
    return hashtable->size;
}

long hashtable_n_elems(Hashtable* hashtable) {
    return hashtable->n_elems;
}

/* Function to insert into the hashtable, returns the hashtable's n_elems or -1 on error */
int hashtable_insert(Hashtable* hashtable, Triple* triple) {
    long h;
    if(!hashtable || !triple) return -1;

    h = hash(triple, hashtable->size);
    if (!chain_search(&hashtable->chain[h], triple)) {
        fprintf(stdout, "Inserting (%ld, %ld, (%d, %d)) into the hashtable position %ld\n",
                triple->a, triple->b, triple->point.x, triple->point.y, h);
        chain_insert(&hashtable->chain[h], triple);
        hashtable->n_elems++;
    } else {
        fprintf(stderr, "Cannot insert element into the hashtable\n");
    }
    return hashtable->n_elems;
}

int hashtable_search(Hashtable* hashtable, Triple* triple) {
    long h;
    if (!hashtable || !triple) return -1;

    h = hash(triple, hashtable->size);
    if (chain_search(&hashtable->chain[h], triple)) {
        fprintf(stdout, "Element (%ld, %ld, (%d, %d)) found into the hashtable position %ld\n",
                triple->a, triple->b, triple->point.x, triple->point.y, h);
        return 1;
    } else {
        fprintf(stdout, "Cannot find element (%ld, %ld, (%d, %d)) into the hashtable\n",
                triple->a, triple->b, triple->point.x, triple->point.y);
        return 0;
    }
}
