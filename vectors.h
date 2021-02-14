#ifndef size_t
#include <stddef.h>
#endif
typedef struct {
  int *array;
  size_t used;
  size_t size;
} intVector;

void initArray(intVector *a, size_t initialSize);
void insertArray(intVector *a, int element);
void freeArray(intVector *a);