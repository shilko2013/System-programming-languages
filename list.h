#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

typedef int T;

typedef struct node {
    struct node *next;
    T *content;
} node;

typedef struct {
    node *first;
} list;

node *init_node(T);

void free_node(node *);

list *list_create();

void list_add_front(T, list *);

void list_add_back(T, list *);

T list_get(size_t, const list *);

void list_free(list *);

size_t list_length(const list *);

list *list_node_at(size_t, const list *);

T list_sum(const list *);

list *list_read(FILE *file, int);

void list_foreach(const list *l, void (*function)(T *));

void list_map_mut(const list *l, T (*function)(const T *));

list *list_map(const list *l, T (*function)(const T *));

T list_reduce(T, void (*binary_function)(const T *, T *), list *);

list *list_iterate(T, size_t, T (*function)(const T *));

int list_save(const list *, const char *, int binary);

int list_load(list *, const char *, int binary);
#endif