#include "memory.h"

/*------------------------------------------------------------------------------
//	Defining static functions
//----------------------------------------------------------------------------*/

/**
 * @ingroup memory_management
 *
 * @brief Align the size
 */
static inline size_t make_align_size(size_t n);

/**
 * @ingroup memory_management
 *
 * @brief Find a suitable block or request memory from the operating system if the block is not found.
 */
static inline mcb_t *find_or_reserve_chunk(size_t size);

/**
 * @ingroup memory_management
 *
 * @brief If the found block is larger than necessary, cut a piece from it (piece size of size).
 */
static inline mcb_t *split(mcb_t *chunk, size_t size);

/**
 * @ingroup memory_management
 *
 * @brief The minimum allocation size is one page.
 */
static inline size_t min_size_for_allocate();

/**
 * @ingroup memory_management
 *
 * @brief If the required size is less than one page, return the page size, otherwise return the size.
 */
static inline size_t make_alloc_size(size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Request memory from the operating system and insert the allocated memory into the list.
 */
static inline mcb_t *allocate_chunk(mcb_t *prev, size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Return size + sizeof(mcb_t)
 */
static inline size_t chunk_size(size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Return pointer to data from mcb
 */
static inline void *mcb_t_data(mcb_t *mcb);

/**
 * @ingroup memory_management
 *
 * @brief Return pointer to mcb from memmory block
 */
static inline mcb_t *mcb_t_metadata(void *ptr);

static mcb_t *heap = NULL; ///< first bite of heap

/*------------------------------------------------------------------------------
//	Implementation malloc and free
//----------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------
//	Implementation static functions
//----------------------------------------------------------------------------*/

static inline size_t make_align_size(size_t n) {
  return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

static inline mcb_t *find_or_reserve_chunk(size_t size) {
  mcb_t *previous = NULL;
  if (!heap) {
    heap = allocate_chunk(previous, size);
    return mcb_t_data(heap);
  }

  // Scan the heap and find a suitable block
  mcb_t *current = heap;
  while (current) {
    if (current->is_available && current->size >= chunk_size(size)) {
      if (current->size == size)
        break;
      else {
        /*
          Split the current chunk to two part(current and new)
          current->size = size
          new->size = current->size - (size + sizeof(mcb_t))
        */
        split(current, size);
        break;
      }
    }
    previous = current;
    current = current->next;
  }
  if (current)
    current->is_available = false;
  else
    current = allocate_chunk(previous, size);

  return current;
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

static inline mcb_t *allocate_chunk(mcb_t *prev, size_t size) {
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

static inline size_t make_alloc_size(size_t size) {
  return size >= min_size_for_allocate() ? size : min_size_for_allocate();
}

static inline size_t min_size_for_allocate() {
  return (size_t)(1 * sysconf(_SC_PAGESIZE));
}

static inline size_t chunk_size(size_t size) { return size + sizeof(mcb_t); }

static inline void *mcb_t_data(mcb_t *mcb) { return mcb ? mcb + 1 : NULL; }

static inline mcb_t *mcb_t_metadata(void *ptr) {
  return ptr ? (mcb_t *)ptr - 1 : NULL;
}
