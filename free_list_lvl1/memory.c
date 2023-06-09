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
 * @brief Find a suitable block or request memory from the operating system if
 * the block is not found.
 */
static inline mcb_t *find_or_reserve_chunk(size_t size);

/**
 * @ingroup memory_management
 *
 * @brief If the found block is larger than necessary, cut a piece from it
 * (piece size of size).
 */
static inline mcb_t *split(mcb_t *chunk, size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Insert new node to the free list.
 */
static inline void insert(mcb_t **head, mcb_t *new_node);

/**
 * @ingroup memory_management
 *
 * @brief Remove the node from the free list.
 */
static inline void remove(mcb_t **head, mcb_t *node);

/**
 * @ingroup memory_management
 *
 * @brief Get address of block.
 */
static inline char *address(mcb_t *node);

/**
 * @ingroup memory_management
 *
 * @brief Scan the free list  and find adjacent blocks. If
 * the blocks can be combined, do it. Two adjacent free blocks can be combined
 * into one.
 */
static inline void merge_adjacent_chunks(mcb_t **head);

/**
 * @ingroup memory_management
 *
 * @brief The minimum allocation size is one page.
 */
static inline size_t min_size_for_allocate();

/**
 * @ingroup memory_management
 *
 * @brief If the required size is less than one page, return the page size,
 * otherwise return the size.
 */
static inline size_t make_alloc_size(size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Request memory from the operating system and insert the allocated
 * memory into the list.
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

static inline bool is_adjacent(mcb_t *first, mcb_t *second);

static inline void merge(mcb_t *first, mcb_t *second);

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
  mcb_t *free_node = mcb_t_metadata(ptr);
  insert(&heap, free_node);
  merge_adjacent_chunks(&heap);
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
    if (current->size >= chunk_size(size)) {
      remove(&heap, current);

      if (current->size == size)
        break;
      else {
        /*
          Split the current chunk to two part(current and new)
          current->size = size
          new->size = current->size - (size + sizeof(mcb_t))
        */
        mcb_t *new_node = split(current, size);
        insert(&heap, new_node);
        break;
      }
    }
    previous = current;
    current = current->next;
  }
  if (!current)
    current = allocate_chunk(previous, size);

  return current;
}

static inline mcb_t *split(mcb_t *old_chunk, size_t size) {
  char *data = mcb_t_data(old_chunk);

  mcb_t *new_chunk = (mcb_t *)(data + size);
  new_chunk->size = old_chunk->size - chunk_size(size);
  new_chunk->next = NULL;
  new_chunk->prev = NULL;

  old_chunk->size = size;

  return new_chunk;
}

static inline void remove(mcb_t **head, mcb_t *removed) {
  mcb_t *prev = removed->prev;
  mcb_t *next = removed->next;

  // first element
  if (prev == NULL) {
    if (next)
      *head = next;
    else
      *head = NULL;
  } else {
    prev->next = removed->next;
    if (next)
      next->prev = removed->prev;
  }
}

static inline void insert(mcb_t **head, mcb_t *node) {
  if (*head == NULL)
    *head = node;
  else if (address(*head) > address(node)) {
    (*head)->prev = node;
    node->next = *head;
    *head = node;
  } else {
    mcb_t *curr = *head;
    while (curr->next && address(curr->next) < address(node)) {
      curr = curr->next;
    }
    node->next = curr->next;
    node->prev = curr;
    curr->next = node;
  }
}

static inline void merge_adjacent_chunks(mcb_t **head) {
  mcb_t *curr = *head;
  while (curr) {
    if (curr->next == NULL)
      break;
    if (is_adjacent(curr, curr->next))
      merge(curr, curr->next);
    curr = curr->next;
  }
}

static inline bool is_adjacent(mcb_t *first, mcb_t *second) {
  char *first_address = address(first);
  char *second_address = address(second);

  return (first_address + first->size + sizeof(mcb_t) == second_address);
}

static inline char *address(mcb_t *node) { return (char *)node; }

static inline void merge(mcb_t *current, mcb_t *next) {
  current->size += chunk_size(next->size);
  current->next = next->next;
  if (next->next)
    next->next->prev = current;
}

static inline mcb_t *allocate_chunk(mcb_t *prev, size_t size) {
  size = make_alloc_size(size);

  mcb_t *last_node = (mcb_t *)sbrk((intptr_t)size);

  if (last_node == (mcb_t *)-1)
    return NULL;

  if (prev)
    prev->next = last_node;

  last_node->size = size;
  last_node->prev = prev;
  last_node->next = NULL;
  return last_node;
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