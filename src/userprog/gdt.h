#ifndef USERPROG_GDT_H
#define USERPROG_GDT_H

#include "threads/loader.h"

/* Segment selectors.
   More selectors are defined by the loader in loader.h. */
#define SEL_UCSEG       0x1B    
#define SEL_UDSEG       0x23    
#define SEL_TSS         0x28    
#define SEL_CNT         6       

void gdt_init (void);

#endif 
