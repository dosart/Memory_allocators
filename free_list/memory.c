#include "memory.h"

extern fl_mcb_t *global_head;

static inline size_t fl_make_aligin(size_t size);

static inline fl_mcb_t *fl_find_suitable_block(fl_mcb_t *head, size_t size);

static inline size_t get_system_page_size();

static inline size_t get_alloc_size(size_t size);

static inline fl_mcb_t *fl_request_space_of_os(size_t size);

static inline void fl_mark_new_block(fl_mcb_t *new_block, size_t size);

void fl_print_aligin(size_t size) {
  printf("ALIGN(%ld) = %ld\n", size, fl_make_aligin(size));
}

void fl_free(void *ptr) {
  fl_insert(fl_get_header_block(ptr));
  /*
   The heap has been changed. Try to merge adjacent blocks to reduce
   fragmentation
  */
  fl_merge_adjacent_blocks(global_head);
}

void *fl_malloc(size_t size) {
  // size = fl_make_aligin(size);
  fl_mcb_t *suitable_block = fl_find_suitable_block(global_head, size);
  if (suitable_block)
    return fl_get_memory_block(suitable_block);
  /*
    We have not found a suitable memory block, so we ask the OS to allocate
    memory
  */
  fl_mcb_t *new_block = fl_request_space_of_os(size);
  if (!new_block)
    return NULL;

  fl_mark_new_block(new_block, size);
  return fl_get_memory_block(new_block);
}

static inline size_t fl_make_aligin(size_t size) {
  return (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
}

static inline fl_mcb_t *fl_find_suitable_block(fl_mcb_t *head, size_t size) {
  fl_mcb_t *current = head;
  size_t suitable_size = size + sizeof(fl_mcb_t);
  while (current) {
    if (current->size >= suitable_size) {
      fl_remove(current);

      if (current->size == size)
        return current;

      fl_mcb_t *new_node = fl_cut_off(current, size);
      fl_insert(new_node);
      return current;
    }
    current = current->next;
  }
  return NULL;
}

static inline fl_mcb_t *fl_request_space_of_os(size_t size) {
  size_t alloc_size = get_alloc_size(size);
  void *new_chunk = sbrk((intptr_t)alloc_size);
  if (new_chunk == (void *)-1)
    return NULL;
  return new_chunk;
}

static inline size_t get_system_page_size() {
  return (size_t)sysconf(_SC_PAGESIZE);
}

static inline size_t get_min_aloc_size_from_os() {
  return 3 * get_system_page_size();
}

static inline size_t get_alloc_size(size_t size) {
  return size >= get_min_aloc_size_from_os() ? size + sizeof(fl_mcb_t)
                                             : get_min_aloc_size_from_os();
}

static inline void fl_mark_new_block(fl_mcb_t *new_block, size_t size) {
  new_block->next = NULL;
  new_block->prev = NULL;

  size_t alloc_size = get_alloc_size(size);
  new_block->size = alloc_size - sizeof(fl_mcb_t);

  if (alloc_size > size + sizeof(fl_mcb_t)) {
    fl_insert(fl_cut_off(new_block, size));
  }
}