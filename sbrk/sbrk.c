#include <stdlib.h>
#include <sys/mman.h>

static size_t HEAP_SIZE = 4096;

static char *heap;

static char *brkp = NULL;
static char *endp = NULL;

void sbrk_init() __attribute__((constructor));
void sbrk_destroy() __attribute__((destructor));

void *simple_sbrk(size_t size);

// Creating a read write, private, not file backed memory allocation
void sbrk_init() {
  heap = mmap(
      /* addr= */ NULL, // The OS decides where to place the allocated memory
      /* length= */ HEAP_SIZE,
      /* prot= */ (PROT_READ | PROT_WRITE),
      /* flags= */ (MAP_PRIVATE | MAP_ANONYMOUS),
      /* fd= */ -1, // Passing an invalid file descriptor
      /* offset= */ 0);
  brkp = heap;
  endp = brkp + HEAP_SIZE;
}

void sbrk_destroy() {
  munmap(/* addr= */ heap,
         /* length= */ HEAP_SIZE);
}

void *simple_sbrk(size_t size) {
  if (size == 0)
    return (void *)brkp;

  void *free = (void *)brkp;

  brkp += size;
  if (brkp >= endp)
    return NULL;
  return free;
}
