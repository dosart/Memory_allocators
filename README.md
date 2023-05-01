# Memory allocators

Custom C implementation of the malloc\free  library functions.

It exposes the following methods:

```c
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
```

The general idea is that for every chunk of memory allocated by malloc, the we store a few bytes (specifically sizeof(mcb_t)) of metadata about that chunk right beforehand. We scan the entire heap and  stop when we see  the first available chunk. If we have not found a suitable fragment, we use the sbrk system call to request memory from the operating system. The sbrk directory contains a custom implementation of this function:

```c
void *sbrk(size_t size);
```

## Simple_allocator_lvl1

This project contains the implementation this idea.

## Simple_allocator_lvl2

This project contains the implementation  Simple_allocator_lvl1 and also aligned and blocks splitting.

For faster access, a memory block should be aligned, and usually by the size of the machine word. 
if we found a block of a suitable size, we just use it. This might be inefficient in case if a found block is much larger than the requested one. We shall now implement a procedure of splitting a larger free block, taking from it only a smaller chunk, which is requested. The other part stays free, and can be used in further allocation requests.


## Free_list_lvl1

This project contains the implementation Simple_allocator_lvl2 and also sorting the list of free blocks to combine adjacent blocks into one.
