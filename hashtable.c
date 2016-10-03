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

int chain_search(Chain* chain, Triple* triple) {
    List* temp;
    if (!chain || !triple) return -1;

    temp = chain->list;
    while(temp) {
        /* Change this. It should use a hash obtained from point->x and point->y values */
        int is_x_equal = mpz_cmp(temp->data->point.x, triple->point.x);
        int is_y_equal = mpz_cmp(temp->data->point.y, triple->point.y);
        if(is_x_equal == 0 && is_y_equal == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

/* chain_collided will look for a collision of a triple (arg triple) into a
 * chain (arg chain) and will copy the collided triple into r_triple
 */
int chain_collided(Chain* chain, Triple* triple, Triple* r_triple) {
    List* temp;
    if (!chain || !triple) {
        fprintf(stderr, "Invalid args: chain_collided()\n");
        return -1;
    }

    temp = chain->list;
    while(temp) {
        int is_x_equal = mpz_cmp(temp->data->point.x, triple->point.x);
        int is_y_equal = mpz_cmp(temp->data->point.y, triple->point.y);
        if(is_x_equal == 0 && is_y_equal == 0) {
            r_triple = temp->data;
            r_triple->point = temp->data->point;
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
int hashtable_insert(Hashtable* hashtable, Triple* triple) {
    long h;
    if(!hashtable || !triple) return -1;

    h = hash(triple, hashtable->size);
    /* OBS: this chain search will be duplicated into Pollard's rho algorithm.
     * Maybe this should be deleted
     */
    if (!chain_search(&hashtable->chain[h], triple)) {
        gmp_printf("Inserting (%Zd, %Zd, (%Zd, %Zd)) into the hashtable position %ld\n",
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
        gmp_printf("Element (%Zd, %Zd, (%Zd, %Zd)) found into the hashtable position %ld\n",
                triple->a, triple->b, triple->point.x, triple->point.y, h);
        return 1;
    } else {
        gmp_printf("Cannot find element (%Zd, %Zd, (%Zd, %Zd)) into the hashtable\n",
                triple->a, triple->b, triple->point.x, triple->point.y);
        return 0;
    }
}

int hashtable_collide(Hashtable* hashtable, Point* point, Triple* triple) {
    mpz_t dummy_x, dummy_y;
    Triple* dummy_triple;
    long h;

    if(!hashtable || !point) {
        fprintf(stderr, "Invalid args: hashtable_collide()\n");
        return -1;
    }

    mpz_init_set_ui(dummy_x, 0);
    mpz_init_set_ui(dummy_y, 0);
    dummy_triple = triple_create(dummy_x, dummy_y, *point);

    h = hash(dummy_triple, hashtable->size);
    if (chain_collided(&hashtable->chain[h], dummy_triple, triple)) {
        fprintf(stdout, "Collision found\n");
    }

    return 0;
}
