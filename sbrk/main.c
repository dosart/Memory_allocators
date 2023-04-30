#include <stddef.h>

extern void sbrk_init();
extern void sbrk_destroy();

extern void *simple_sbrk(size_t size);

int main() {
  sbrk_init();

  simple_sbrk(100);
  sbrk_destroy();
}