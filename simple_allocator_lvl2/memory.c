#include "memory.h"

static mcb_t *heap = NULL; ///< first free byte of memory

static inline void *mcb_t_data(mcb_t *mcb) { return mcb ? mcb + 1 : NULL; }

static inline mcb_t *mcb_t_metadata(void *ptr) {
  return ptr ? (mcb_t *)ptr - 1 : NULL;
}

static inline size_t make_align_size(size_t n) {
  return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

static inline size_t chunk_size(size_t size) { return size + sizeof(mcb_t); }

static inline mcb_t *find_or_reserve_chunk(size_t size);
static inline mcb_t *split(mcb_t *old_chunk, size_t size);

static inline size_t min_size_for_allocate();
static inline size_t make_alloc_size(size_t size);
mcb_t *allocate_chunk(mcb_t *prev, size_t size);

void *simple_malloc(size_t size) {
  if (size == 0)
    return NULL;
  size = make_align_size(size);
  return mcb_t_data(find_or_reserve_chunk(size));
}

void simple_free(void *ptr) {
  mcb_t *mcb = mcb_t_metadata(ptr);
  if (!mcb)
    return;
  mcb->is_available = true;
}

static inline mcb_t *find_or_reserve_chunk(size_t size) {
  mcb_t *prev_chunk = NULL;
  if (!heap) {
    heap = allocate_chunk(prev_chunk, size);
    return mcb_t_data(heap);
  }

  // Scan the heap for holes large enough for the chunk we want.
  mcb_t *chunk = heap;
  while (chunk) {
    if (chunk->is_available && chunk->size >= chunk_size(size)) {
      if (chunk->size == size)
        break;
      else {
        split(chunk, size);
        break;
      }
    }
    prev_chunk = chunk;
    chunk = chunk->next;
  }
  if (chunk)
    chunk->is_available = false;
  else
    chunk = allocate_chunk(prev_chunk, size);

  return chunk;
}

static inline mcb_t *split(mcb_t *old_chunk, size_t size) {
  char *data = mcb_t_data(old_chunk);

  mcb_t *new_chunk = (mcb_t *)(data + size);
  new_chunk->size = old_chunk->size - chunk_size(size);
  new_chunk->is_available = true;
  new_chunk->next = old_chunk->next;

  old_chunk->size = size;
  old_chunk->next = new_chunk;
  return new_chunk;
}

static inline size_t min_size_for_allocate() {
  return (size_t)(1 * sysconf(_SC_PAGESIZE));
}

static inline size_t make_alloc_size(size_t size) {
  return size >= min_size_for_allocate() ? size : min_size_for_allocate();
}

mcb_t *allocate_chunk(mcb_t *prev, size_t size) {
  size = make_alloc_size(size);

  mcb_t *last_chunk = (mcb_t *)sbrk((intptr_t)size);

  if (last_chunk == (mcb_t *)-1)
    return NULL;

  if (prev)
    prev->next = last_chunk;

  last_chunk->size = size;
  last_chunk->is_available = false;
  last_chunk->next = NULL;
  return last_chunk;
}