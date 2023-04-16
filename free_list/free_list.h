#ifndef FREE_LIST_H
#define FREE_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct __mcb_t_t {
  size_t size;
  struct __mcb_t_t *next;
  struct __mcb_t_t *prev;
} fl_mcb_t;

void fl_remove_node(fl_mcb_t *node);

void fl_remove_first();

void fl_insert(fl_mcb_t *node);

void fl_insert_first(fl_mcb_t *new_node);

bool fl_is_empty();

fl_mcb_t *fl_cut_off(fl_mcb_t *node, size_t size);

void *fl_get_memory_block(fl_mcb_t *ptr);

#endif