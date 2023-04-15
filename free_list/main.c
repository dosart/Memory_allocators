#include "memory.h"

int main() {
  for (size_t i=1; i<=32; i+=2)
    fl_print_aligin(i);
  
  fl_malloc(5);
  return 0;
}