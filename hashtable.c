#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"


Triple* triple_create(mpz_t a, mpz_t b, Point point) {
    Triple* t = (Triple*) malloc(sizeof(Triple));

    mpz_init_set(t->a, a);
    mpz_init_set(t->b, b);
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

int chain_search(Chain* chain, Triple* triple, Triple* c_triple) {
    List* temp;
    if (!chain || !triple) return -1;

    temp = chain->list;
    while(temp) {
    /* Change this. It should use a hash obtained from point->x and point->y values */
        int is_x_equal = mpz_cmp(temp->data->point.x, triple->point.x);
        int is_y_equal = mpz_cmp(temp->data->point.y, triple->point.y);
        if(is_x_equal == 0 && is_y_equal == 0) {
            mpz_set(c_triple->a, temp->data->a);
            mpz_set(c_triple->b, temp->data->b);
            mpz_set(c_triple->point.x, temp->data->point.x);
            mpz_set(c_triple->point.y, temp->data->point.y);
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
    unsigned long result;
    mpz_t temp_result;

    mpz_init(temp_result);

    result = mpz_mod_ui(temp_result, triple->point.x, size);
    mpz_clear(temp_result);
    return result;
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
int hashtable_insert(Hashtable* hashtable, Triple* triple, Triple* c_triple) {
    long h;
    if(!hashtable || !triple || !c_triple) return -1;

    h = hash(triple, hashtable->size);
    if (!chain_search(&hashtable->chain[h], triple, c_triple)) {
        gmp_printf("Inserting (%Zd, %Zd, (%Zd, %Zd)) into the hashtable position %ld\n",
                triple->a, triple->b, triple->point.x, triple->point.y, h);
        chain_insert(&hashtable->chain[h], triple);
        hashtable->n_elems++;
    } else {
        gmp_printf("Collision found in point (%Zd, %Zd)\n",
                      triple->point.x, triple->point.y);
        gmp_printf("\ta1 = %Zd, b1 = %Zd\n", triple->a, triple->b);
        gmp_printf("\ta2 = %Zd, b2 = %Zd\n", c_triple->a, c_triple->b);
    }
    return hashtable->n_elems;
}
