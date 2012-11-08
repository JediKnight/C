#include  <stdio.h>

int heapsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
int heapsort_b(void *base, size_t nel, size_t width, int (^compar)(const void *, const void *));
int mergesort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
int mergesort_b(void *base, size_t nel, size_t width, int (^compar)(const void *, const void *));
void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
void qsort_b(void *base, size_t nel, size_t width, int (^compar)(const void *, const void *));
void qsort_r(void *base, size_t nel, size_t width, void *thunk, int (*compar)(void *, const void *, const void *));
