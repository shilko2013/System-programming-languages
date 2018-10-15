#include <stdio.h>
#include <limits.h>
#include "list.h"

#define FILENAME "saved_list"

T square(const T *a) { return *a * *a; }

T cube(const T *a) { return *a * *a * *a; }

void sum(const T *val, T *a) { *a = *val + *a; }

void min(const T *val, T *a) { *a = *a < *val ? *a : *val; }

void max(const T *val, T *a) { *a = *a > *val ? *a : *val; }

void print_space(T *a) { printf("%d ", *a); }

void print_new_line(T *a) { printf("%d\n", *a); }

T absolute(const T *val) { return *val < 0 ? -*val : *val; }

T double_value(const T *val) { return *val * 2; }

void list_print(const list *l, const char *message, void (*function)(T *)) {
    printf("%s", message);
    list_foreach(l, function);
    printf("\n");
}

int main(void) {

    list *l = list_read(stdin,1);
    list_print(l, "list: ", print_space);
    list_print(l, "list: ", print_new_line);
    list *temp = list_map(l, square);
    list_print(temp, "list^2: ", print_space);
    list_free(temp);
    temp = list_map(l, cube);
    list_print(temp, "list^3: ", print_space);
    list_free(temp);
    printf("list_sum: %d\n", list_reduce(0, sum, l));
    printf("list_min: %d\n", list_reduce(INT_MAX, min, l));
    printf("list_max: %d\n", list_reduce(INT_MIN, max, l));
    list_map_mut(l, absolute);
    list_print(l, "list_abs: ", print_space);
    temp = list_iterate(1, 10, double_value);
    list_print(temp, "powers_of_2: ", print_space);
    list_free(temp);
    list_save(l,FILENAME,0);
    list_free(l);
    list_load(l,FILENAME,0);
    list_print(l, "loaded_list: ", print_space);
    list_save(l,FILENAME,1);
    list_free(l);
    list_load(l,FILENAME,1);
    list_print(l, "loaded_binary_list: ", print_space);
    list_free(l);

    return 0;
}