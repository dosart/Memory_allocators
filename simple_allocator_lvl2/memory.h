#ifndef MEMORY_ALLOCATOR_LVL2
#define MEMORY_ALLOCATOR_LVL2

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef intptr_t word_t;

typedef struct __mcb_t_t {
  size_t size; ///< Memory block size
  struct __mcb_t_t *next;
  bool is_available; ///< true is free else false
} mcb_t;

void *simple_malloc(size_t size);

void simple_free(void *ptr);

#endif