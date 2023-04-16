#include "free_list.h"

fl_mcb_t *global_head = NULL;
void fl_remove_node(fl_mcb_t *node) {
  if (node == NULL)
    return;
  if (node == global_head)
    fl_remove_first();
  else {
    if (node->prev)
      node->prev->next = node->next;
    if (node->next)
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

void *fl_get_memory_block(fl_mcb_t *ptr) {
  return (char *)ptr + sizeof(fl_mcb_t);
}