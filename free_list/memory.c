#include "memory.h"

static inline size_t aligin(size_t size) {return (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);}

static inline size_t get_system_page_size() { return (size_t)sysconf(_SC_PAGESIZE);}
static inline size_t get_allc_init() { return 3 * get_system_page_size();}
static inline size_t get_alloc_size(size_t size) {return size >= 3 * get_allc_init() ? size + sizeof(fl_mcb_t) : 3 * get_allc_init();} 

static inline void* get_memory(fl_mcb_t * ptr) {return (char*)ptr + sizeof(fl_mcb_t);}



static fl_mcb_t *global_head = NULL;

void fl_print_aligin(size_t size)
{ printf("ALIGN(%ld) = %ld\n", size, aligin(size)); }

void* fl_malloc(size_t size)
{
  size = aligin(size);

  fl_mcb_t * current_mcb = global_head;
  void *current_memory_block = NULL;
  while (current_mcb)
  {

  }

  size_t	alloc_size = get_alloc_size(size);
  if ((sbrk((intptr_t)alloc_size)) == (void *)-1) 
    return NULL;
  
  return current_mcb;
}