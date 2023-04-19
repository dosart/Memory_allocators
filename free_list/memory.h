/*!
@file
@defgroup memory_management
@brief  Header file for malloc
This file contains functions implementation from libs (malloc.h)
*/
#ifndef FREE_LIST_H_
#define FREE_LIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "free_list.h"

void fl_print_aligin(size_t size);

void *fl_malloc(size_t size);

void fl_free(void *ptr);

#endif // FREE_LIST_H_