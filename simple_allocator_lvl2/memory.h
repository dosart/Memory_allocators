/*!
@file
@defgroup memory_management
@brief  Header file for malloc
*/

#ifndef MEMORY_ALLOCATOR_LVL2
#define MEMORY_ALLOCATOR_LVL2

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
 */
typedef struct __mcb_t_t {
  size_t size; ///< Memory block size
  struct __mcb_t_t *next;
  bool is_available; ///< True is free else false
} mcb_t;

/**
 --------------------   ----------------   ----------------   ------------------
 node1:                 user data          node2:             user data
 size = 10;                                size = 8;
 is_available = 0;                         is_available = 1
 next = node2                              next = NULL
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