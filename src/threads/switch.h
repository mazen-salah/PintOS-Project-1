#ifndef THREADS_SWITCH_H
#define THREADS_SWITCH_H

#ifndef __ASSEMBLER__

struct switch_threads_frame 
  {
    uint32_t edi;               
    uint32_t esi;               
    uint32_t ebp;               
    uint32_t ebx;               
    void (*eip) (void);         
    struct thread *cur;         
    struct thread *next;        
  };

/* Switches from CUR, which must be the running thread, to NEXT,
   which must also be running switch_threads(), returning CUR in
   NEXT's context. */
struct thread *switch_threads (struct thread *cur, struct thread *next);


struct switch_entry_frame
  {
    void (*eip) (void);
  };

void switch_entry (void);

/* Pops the CUR and NEXT arguments off the stack, for use in
   initializing threads. */
void switch_thunk (void);
#endif


#define SWITCH_CUR      20
#define SWITCH_NEXT     24

#endif 
