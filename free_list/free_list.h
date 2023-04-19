/*!
@file
@ingroup memory_management
@brief The header file contains the implementation of a doubly linked list for
the memory allocator.

The allocated but free memory area is stored in the node of the doubly linked
list.
*/
#ifndef FREE_LIST_H
#define FREE_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

/*!
 * @ingroup memory_management
 *
 * @brief Meta information for a region of memory.
 * This structure describes an allocated but free region of memory.
 */
typedef struct __mcb_t_t {
  size_t size;            ///< Memory region size
  struct __mcb_t_t *next; ///< Pointer to the next memory area (free list node)
  struct __mcb_t_t
      *prev; ///< Pointer to the previous memory area (free list node)
} fl_mcb_t;

/**
 ---------      ------------------     ------------  ----------------------
 node1:         free region memory     node2:        free region of memory
 prev = null;                          prev = node1
 next = node2;                         next = NULL
 ---------      ------------------     ------------ ------------------------
*/

/**
 * @ingroup memory_management
 *
 * @brief Get pointer to fl_mcb_t.
 *
 * @param ptr Pointer to memory.
 *
 * @return Meta information for a region of memory.
 */
fl_mcb_t *fl_get_header_block(void *ptr);

/**
 * @ingroup memory_management
 *
 * @brief Get pointer to memory from fl_mcb_t.
 *
 * @param ptr Meta information for a region of memory.
 *
 * @return pointer to memory
 */
void *fl_get_memory_block(fl_mcb_t *ptr);

/**
 * @ingroup memory_management
 *
 * @brief Remove the node from the free list.
 * The head of the list is a global variable - global_head.
 * Maintains a linked list in a sorted state. List sorted by block start
 * address
 *
 * @param node Node to remove.
 * memory.
 */
void fl_remove(fl_mcb_t *node);

/**
 * @ingroup memory_management
 *
 * @brief Remove node from the beginning.
 * The head of the list is a global variable - global_head.
 * Maintains a linked list in a sorted state. List sorted by block start address
 *
 * @param node Node to remove.
 * memory.
 */
void fl_remove_first();

/**
 * @ingroup memory_management
 *
 * @brief Insert the node to the free list.
 * The head of the list is a global variable - global_head.
 * Maintains a linked list in a sorted state. List sorted by block start address
 *
 * @param node Node to remove.
 * memory.
 */
void fl_insert(fl_mcb_t *node);

/**
 * @ingroup memory_management
 *
 * @brief Insert node at the beginning.
 * The head of the list is a global variable - global_head.
 * Maintains a linked list in a sorted state. List sorted by block start address
 *
 * @param node Node to remove.
 * memory.
 */
void fl_insert_first(fl_mcb_t *new_node);

/**
 * @ingroup memory_management
 *
 * @brief Return true if free list is empty.
 * The head of the list is a global variable - global_head.
 *
 * @return true if free list is empty.
 * memory.
 */
bool fl_is_empty();

/**
 * @ingroup memory_management
 *
 * @brief Cut a piece of size n from the node.
 * The head of the list is a global variable - global_head.
 *
 * @param node The node to be cut off from.
 * @param size The size to be cut.
 *
 * @return New node size node->size - (size + sizeof(fl_mcb_t))
 */
fl_mcb_t *fl_cut_off(fl_mcb_t *node, size_t size);

/**
 * @ingroup memory_management
 *
 * @brief Combine blocks with adjacent addresses in a free list.
 * The function iterates for each block in the free list and combines blocks
 * with adjacent addresses.
 *
 * @param header pointer to head of free list.
 */
void fl_merge_adjacent_blocks(fl_mcb_t *head);

/**
 * @ingroup memory_management
 *
 * @brief Return true if blocks are adjacent.
 *
 * @param current pointer to the one block.
 * @param next pointer to the next adjacent block.
 *
 * @return Return true if blocks are adjacent
 */
bool fl_is_adjacent_blocks(fl_mcb_t *current, fl_mcb_t *next);

/**
 * @ingroup memory_management
 *
 * @brief Combine two blocks with adjacent addresses.
 *
 * @param current pointer to the one block.
 * @param next pointer to the next adjacent block.
 *
 */
void fl_merge(fl_mcb_t *current, fl_mcb_t *next);

/**
 * @ingroup memory_management
 *
 * @brief Return the OS block if it is too big.
 *
 * @param last Pointer to the last block.
 *
 */
void fl_return_last_block_os(fl_mcb_t *last);

/**
 * @ingroup memory_management
 *
 * @brief Returns the minimum block size to delete.
 */
size_t get_min_dealock();

#endif