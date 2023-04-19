#include "memory.h"

int main() {
  for (size_t i = 1; i <= 32; i += 2)
    fl_print_aligin(i);

  size_t *arr = (size_t *)fl_malloc(sizeof(size_t) * 1024);
  for (size_t i = 0; i < 100; i++) {
    arr[i] = i;
  }

  fl_free(arr);
  return 0;
}