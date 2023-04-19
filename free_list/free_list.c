#include "free_list.h"

fl_mcb_t *global_head = NULL;

void fl_remove(fl_mcb_t *node) {
  if (!node->prev) {
    if (node->next) {
      global_head = node->next;
    } else {
      global_head = NULL;
    }
  } else {
    node->prev->next = node->next;
  }
  if (node->next) {
    node->next->prev = node->prev;
  }
}

void fl_remove_first() {
  if (global_head == NULL)
    return;
  if (global_head->next == NULL) ///< Only one element in list
    global_head = NULL;
  else {
    global_head->next->prev = NULL;
    global_head = global_head->next;
  }
}

void fl_insert(fl_mcb_t *new_node) {
  if (fl_is_empty() || (char *)global_head > (char *)new_node)
    fl_insert_first(new_node);
  else {
    fl_mcb_t *current = global_head;
    while (current->next && (char *)current->next < (char *)new_node) {
      current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
  }
}

void fl_insert_first(fl_mcb_t *new_node) {
  if (!fl_is_empty())
    global_head->prev = new_node;

  new_node->next = global_head;
  global_head = new_node;
}

bool fl_is_empty() { return (global_head == NULL); }

fl_mcb_t *fl_cut_off(fl_mcb_t *old_chunk, size_t size) {
  void *memory_block = fl_get_memory_block(old_chunk);

  fl_mcb_t *new_chunk = (fl_mcb_t *)((char *)memory_block + size);
  new_chunk->size = old_chunk->size - (size + sizeof(fl_mcb_t));

  old_chunk->size = size;
  return new_chunk;
}

void fl_merge_adjacent_blocks(fl_mcb_t *head) {
  fl_mcb_t *current = head;
  fl_mcb_t *next = NULL;

  while (current) {
    if (!current->next)
      break;

    next = current->next;
    if (fl_is_adjacent_blocks(current, next))
      fl_merge(current, next);
    current = current->next;
  }
  fl_return_last_block_os(current);
}

bool fl_is_adjacent_blocks(fl_mcb_t *current, fl_mcb_t *next) {
  char *current_address = (char *)current;
  char *next_address = (char *)next;

  return (current_address + current->size + sizeof(fl_mcb_t) == next_address);
}

void fl_merge(fl_mcb_t *current, fl_mcb_t *next) {
  current->size += next->size + sizeof(fl_mcb_t);
  current->next = next->next;
  if (current->next)
    current->next->prev = current;
}

void fl_return_last_block_os(fl_mcb_t *last) {
  char *last_address = (char *)last;
  char *last_valid_address = sbrk(0);

  if (last_address + last->size + sizeof(fl_mcb_t) == last_valid_address &&
      last->size >= get_min_dealock()) {
    fl_remove(last);
    brk(last_address);
  }
}

void *fl_get_memory_block(fl_mcb_t *ptr) {
  return (char *)ptr + sizeof(fl_mcb_t);
}

fl_mcb_t *fl_get_header_block(void *ptr) {
  return (fl_mcb_t *)((char *)ptr - sizeof(fl_mcb_t));
}

size_t get_min_dealock() { return 1 * (size_t)sysconf(_SC_PAGESIZE); }