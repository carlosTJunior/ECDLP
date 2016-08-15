#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"
#include "xalloc.h"

typedef struct chain* chain;

struct table {
    int size;
    int num_elems;
    chain* array;
    ht_key (*elem_key)(ht_elem e);
    bool (*equal)(ht_key k1, ht_key k2);
    int (*hash)(ht_key k, int m);
};

struct list {
    ht_elem data;
    struct list* next;
};
typedef struct list* list;

void list_free(list p, void (*elem_free)(ht_elem e)) {
    list q;
    while (p != NULL) {
        if (p->data != NULL && elem_free != NULL)
            (*elem_free)(p->data);
        q = p->next;
        free(p);
        p = q;
    }
}

struct chain {
    list list;
};

bool is_chain(chain C) {
    return C != NULL;
}

chain chain_new() {
    chain C = xmalloc(sizeof(struct chain));
    C->list = NULL;
    assert(is_chain(C));
    return C;
}

list chain_find(table H, chain C, ht_key k) {
    assert(is_chain(C));
    list p = C->list;
    while (p != NULL) {
        if ((*H->equal)(k, (*H->elem_key)(p->data)))
            return p;
        p = p->next;
    }
    return NULL;
}

ht_elem chain_insert(table H, chain C, ht_elem e)
{
    assert(is_chain(C) && e != NULL);
    list p = chain_find(H, C, (*H->elem_key)(e));
    if (p == NULL) {
        list new_item = xmalloc(sizeof(struct list));
        new_item->data = e;
        new_item->next = C->list;
        C->list = new_item;
        assert(is_chain(C));
        return NULL;
    } else {
        ht_elem old_e = p->data;
        p->data = e;
        assert(is_chain(C));
        return old_e;
    }
}

ht_elem chain_search(table H, chain C, ht_key k) {
    assert(is_chain(C));
    list p = chain_find(H, C, k);
    if (p == NULL) return NULL;
    else return p->data;
}

void chain_free(chain C, void (*elem_free)(ht_elem e)) {
    assert(is_chain(C));
    list_free(C->list, elem_free);
    free(C);
}

/* h = hash (the key of the elem) and m is the size of the table */
bool is_h_chain(table H, chain C, int h, int m) {
    assert(0 <= h && h < m);
    if (C == NULL) return false;
    list p = C->list;
    while (p != NULL) {
        if (p->data == NULL) return false;
        if ((*H->hash)((*H->elem_key)(p->data), m) != h)
            return false;
        p = p->next;
    }
    return true;
}

bool is_table(table H) {
    int i, m;
    if (H == NULL) return false;
    m = H->size;
    for (i = 0; i < m; i++) {
        chain C = H->array[i];
        if (!(C == NULL || is_h_chain(H, C, i, m))) return false;
    }
    return true;
}

table table_new(int init_size,
                ht_key (*elem_key)(ht_elem e),
                bool (*equal)(ht_key k1, ht_key k2),
                int (*hash)(ht_key k, int m))
{
    assert(init_size > 1);
    chain* A = xcalloc(init_size, sizeof(chain));
    table H = xmalloc(sizeof(struct table));
    H->size = init_size;
    H->num_elems = 0;
    H->array = A;
    H->elem_key = elem_key;
    H->equal = equal;
    H->hash = hash;
    assert(is_table(H));
    return H;
}

ht_elem table_insert(table H, ht_elem e) {
    assert(is_table(H));
    ht_elem old_e;
    ht_key k = (*H->elem_key)(e);
    int h = (*H->hash)(k, H->size);
    if (H->array[h] == NULL)
        H->array[h] = chain_new();
    old_e = chain_insert(H, H->array[h], e);
    if (old_e != NULL) return old_e;
    H->num_elems++;
    assert(is_table(H));
    assert(table_search(H, (*H->elem_key)(e)) == e);
    return NULL;
}

ht_elem table_search(table H, ht_key k) {
    assert(is_table(H));
    int h = (*H->hash)(k, H->size);
    if (H->array[h] == NULL) return NULL;
    ht_elem e = chain_search(H, H->array[h], k);
    assert(e == NULL || (*H->equal)((*H->elem_key)(e), k));
    return e;
}

void table_free(table H, void (*elem_free)(ht_elem e))
{
    assert(is_table(H));
    int i;
    for (i = 0; i < H->size; i++) {
        chain C = H->array[i];
        if (C != NULL)
            chain_free(C, elem_free);
    }
    free(H->array);
    free(H);
}
