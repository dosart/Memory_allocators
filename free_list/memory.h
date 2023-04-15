/*!
@file
@defgroup memory_management
@brief  Header file for malloc
This file contains functions implementation from libs (malloc.h)
*/
#ifndef FREE_LIST_H_
#define FREE_LIST_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void fl_print_aligin(size_t size);

typedef struct __mcb_t_t {
	size_t		size;
	struct __mcb_t_t   *next;
	struct __mcb_t_t   *prev;
}	fl_mcb_t;

void* fl_malloc(size_t size);


#endif // FREE_LIST_H_
