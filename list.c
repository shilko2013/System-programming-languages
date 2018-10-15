#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "list.h"

int list_save(const list *l, const char *filename, int binary) {
    char *mode;
    if (binary)
        mode = "wb";
    else
        mode = "w";
    if (!l || !l->first)
        return 0;
    FILE *file = fopen(filename, mode);
    if (file == NULL)
        return 0;
    node *n = l->first;
    while (n->next) {
        fprintf(file, " %d", *n->content);
        n = n->next;
    }
    fprintf(file, " %d", *n->content);
    fclose(file);
    if (ferror(file))
        return 0;
    return 1;
}

T list_reduce(T start_value, void (*binary_function)(const T *, T *), list *l) {
    T accumulator = start_value;
    if (!l || !binary_function || !l->first)
        return accumulator;
    node *n = l->first;
    while (n->next) {
        (*binary_function)(n->content, &accumulator);
        n = n->next;
    }
    (*binary_function)(n->content, &accumulator);
    return accumulator;
}

void list_map_mut(const list *l, T (*function)(const T *)) {
    if (!l || !function || !l->first)
        return;
    node *n = l->first;
    while (n->next) {
        *n->content = (*function)(n->content);
        n = n->next;
    }
    *n->content = (*function)(n->content);
}

void list_foreach(const list *l, void (*function)(T *)) {
    if (!l || !function || !l->first)
        return;
    node *n = l->first;
    while (n->next) {
        (*function)(n->content);
        n = n->next;
    }
    (*function)(n->content);
}

node *init_node(T content) {
    node *n = malloc(sizeof(node *));
    n->content = malloc(sizeof(T *));
    *n->content = content;
    n->next = NULL;
    return n;
}

void free_node(node *n) {
    if (!n)
        return;
    free(n->content);
    free(n);
}

list *list_create() {
    list *l = malloc(sizeof(list *));
    l->first = NULL;
    return l;
}

void list_add_front(T content, list *l) {
    if (!l)
        return;
    node *n = init_node(content);
    if (l->first)
        n->next = l->first;
    l->first = n;
}

void list_add_back(T content, list *l) {
    if (!l)
        return;
    node *n = init_node(content);
    node *last = l->first;
    if (!last) {
        l->first = n;
        return;
    }
    while (last->next)
        last = last->next;
    last->next = n;
}

void list_free(list *l) {
    if (!l)
        return;
    node *n = l->first;
    if (n) {
        while (n->next) {
            node *temp = n;
            n = n->next;
            free_node(temp);
        }
        free_node(n);
    }
    free(l);
}

size_t list_length(const list *l) {
    if (!l)
        return 0;
    size_t count = 0;
    node *n = l->first;
    if (!n)
        return count;
    ++count;
    while (n->next) {
        n = n->next;
        ++count;
    }
    return count;
}

list *list_node_at(size_t index, const list *l) {
    if (index + 1 > list_length(l))
        return NULL;
    list *result = malloc(sizeof(list *));
    if (!index) {
        memcpy(result, l, sizeof(list));
        return result;
    }
    node *n = l->first;
    for (size_t i = 0; i < index; ++i)
        n = n->next;
    memcpy(result, n, sizeof(list));
    result->first = n;
    return result;
}

T list_sum(const list *l) {
    T sum = 0;
    if (!l || !l->first)
        return sum;
    node *n = l->first;
    sum = *l->first->content;
    while (n->next) {
        n = n->next;
        sum += *n->content;
    }
    return sum;
}

list *list_iterate(T init_value, size_t length, T (*function)(const T *)) {
    list *l = list_create();
    T accumulator = init_value;
    for (size_t i = 0; i < length; ++i) {
        list_add_back(accumulator, l);
        accumulator = (*function)(&accumulator);
    }
    return l;
}

list *list_map(const list *l, T (*function)(const T *)) {
    if (!l || !function || !l->first)
        return NULL;
    list *result = list_create();
    node *n = l->first;
    while (n->next) {
        list_add_back((*function)(n->content), result);
        n = n->next;
    }
    list_add_back((*function)(n->content), result);
    return result;
}

list *list_read(FILE *file, int reverse_mode) {
    list *l = list_create();
    T number;
    int ch = 'y';
    do {
        fscanf(file, "%d", &number);
        if (reverse_mode)
            list_add_front(number, l);
        else
            list_add_back(number, l);
        ch = fgetc(file);
    } while (ch != '\n' && ch != '\r' && ch != EOF);
    return l;
}

int list_load(list *l, const char *filename, int binary) {
    char *mode;
    if (binary)
        mode = "rb";
    else
        mode = "r";
    FILE *file = fopen(filename, mode);
    if (file == NULL)
        return 0;
    l = list_read(file, 0);
    fclose(file);
    if (ferror(file))
        return 0;
    return 1;
}

T list_get(size_t index, const list *l) {
    if (!l)
        return 0;
    if (index + 1 > list_length(l))
        return 0;
    node *n = l->first;
    for (size_t i = 0; i < index; ++i)
        n = n->next;
    return *n->content;
}