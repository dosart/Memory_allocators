/*!
@file
@defgroup memory_management
@brief  Header file for malloc
*/

#ifndef FREE_LIST_LVL1
#define FREE_LIST_LVL1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef intptr_t word_t;

/*!
 * @ingroup memory_management
 *
 * @brief Memory block characteristic
 * Save the free blocks in the linked list. Each block has a pointer to the next
 * block and the previous block.
 * The pointer to the beginning of the linked list, stored at the beginning of
 * the heap
 */
typedef struct __mcb_t_t {
  size_t size; ///< Memory block size
  struct __mcb_t_t *next;
  struct __mcb_t_t *prev;
} mcb_t;

/**
 --------------------   ----------------   ----------------   ------------------
 node1:                 user data          node2:             user data
 size = 10;                                size = 8;

 next = node2                              next = NULL
 prev = NULL                               prev = node1
 --------------------   ----------------   ----------------   ------------------
*/

/**
 * @ingroup memory_management
 *
 * @brief Allocates size bytes of uninitialized storage.
 *
 * @param size Number of bytes to allocate.
 *
 * @returns On success, returns the pointer to the beginning of newly allocated
 * memory.
 */
void *simple_malloc(size_t size);

/**
 * @ingroup memory management
 *
 * @brief Deallocates the space previously allocated by malloc(), calloc(),
 * realoc().
 *
 * @param ptr Pointer to the memory to deallocate.
 */
void simple_free(void *ptr);

#endif