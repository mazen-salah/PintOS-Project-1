/* Expand the stack by 32 bytes all at once using the PUSHA
   instruction.
   This must succeed. */

#include <string.h>
#include "tests/arc4.h"
#include "tests/cksum.h"
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  asm volatile
    ("movl %%esp, %%eax;"        
     "andl $0xfffff000, %%esp;"  
     "pushal;"                   
     "movl %%eax, %%esp"         
     : : : "eax");               
}
