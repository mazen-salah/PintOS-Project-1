#ifndef THREADS_PALLOC_H
#define THREADS_PALLOC_H

#include <stddef.h>


enum palloc_flags
  {
    PAL_ASSERT = 001,           
    PAL_ZERO = 002,             
    PAL_USER = 004              
  };

void palloc_init (size_t user_page_limit);
void *palloc_get_page (enum palloc_flags);
void *palloc_get_multiple (enum palloc_flags, size_t page_cnt);
void palloc_free_page (void *);
void palloc_free_multiple (void *, size_t page_cnt);

#endif 
