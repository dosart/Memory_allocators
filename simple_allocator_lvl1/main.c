#include "memory.h"

int main() {
  int *arr = simple_malloc(sizeof(int) * 1000);
  for (int i = 0; i <= 1000; i++) {
    arr[i] = i;
  }

  simple_free(arr);
  return 0;
}