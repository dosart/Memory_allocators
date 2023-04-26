#include "memory.h"

int main() {
  char *str1, *str2, *str3;
  int *arr1, *arr2;

  str1 = simple_malloc(1);
  str2 = simple_malloc(4);
  str3 = simple_malloc(8);

  simple_free(str3);
  simple_free(str2);
  simple_free(str1);

  arr1 = (int *)simple_malloc(sizeof(int) * 1024);
  arr2 = (int *)simple_malloc(sizeof(int) * 2048);

  simple_free(arr1);
  simple_free(arr2);

  str1 = simple_malloc(32);
  str2 = simple_malloc(44);
  str3 = simple_malloc(1);

  simple_free(str3);
  simple_free(str2);
  simple_free(str1);

  arr1 = simple_malloc(sizeof(int) * 102);
  arr2 = simple_malloc(sizeof(int) * 20);

  simple_free(arr1);
  simple_free(arr2);

  return 0;
}