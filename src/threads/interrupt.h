#ifndef THREADS_INTERRUPT_H
#define THREADS_INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>


enum intr_level 
  {
    INTR_OFF,             
    INTR_ON               
  };

enum intr_level intr_get_level (void);
enum intr_level intr_set_level (enum intr_level);
enum intr_level intr_enable (void);
enum intr_level intr_disable (void);


struct intr_frame
  {
    /* Pushed by intr_entry in intr-stubs.S.
       These are the interrupted task's saved registers. */
    uint32_t edi;               
    uint32_t esi;               
    uint32_t ebp;               
    uint32_t esp_dummy;         
    uint32_t ebx;               
    uint32_t edx;               
    uint32_t ecx;               
    uint32_t eax;               
    uint16_t gs, :16;           
    uint16_t fs, :16;           
    uint16_t es, :16;           
    uint16_t ds, :16;           

    
    uint32_t vec_no;            

    /* Sometimes pushed by the CPU,
       otherwise for consistency pushed as 0 by intrNN_stub.
       The CPU puts it just under `eip', but we move it here. */
    uint32_t error_code;        

    /* Pushed by intrNN_stub in intr-stubs.S.
       This frame pointer eases interpretation of backtraces. */
    void *frame_pointer;        

    /* Pushed by the CPU.
       These are the interrupted task's saved registers. */
    void (*eip) (void);         
    uint16_t cs, :16;           
    uint32_t eflags;            
    void *esp;                  
    uint16_t ss, :16;           
  };

typedef void intr_handler_func (struct intr_frame *);

void intr_init (void);
void intr_register_ext (uint8_t vec, intr_handler_func *, const char *name);
void intr_register_int (uint8_t vec, int dpl, enum intr_level,
                        intr_handler_func *, const char *name);
bool intr_context (void);
void intr_yield_on_return (void);

void intr_dump_frame (const struct intr_frame *);
const char *intr_name (uint8_t vec);

#endif 
