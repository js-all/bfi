#include <stddef.h>
#include <stdlib.h>

typedef struct {
  int *array;
  size_t used;
  size_t size;
} intVector;

void initArray(intVector *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(intVector *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeArray(intVector *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}