#ifndef THREADS_IO_H
#define THREADS_IO_H

#include <stddef.h>
#include <stdint.h>


static inline uint8_t
inb (uint16_t port)
{
  
  uint8_t data;
  asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
  return data;
}

/* Reads CNT bytes from PORT, one after another, and stores them
   into the buffer starting at ADDR. */
static inline void
insb (uint16_t port, void *addr, size_t cnt)
{
  
  asm volatile ("rep insb" : "+D" (addr), "+c" (cnt) : "d" (port) : "memory");
}


static inline uint16_t
inw (uint16_t port)
{
  uint16_t data;
  
  asm volatile ("inw %w1, %w0" : "=a" (data) : "Nd" (port));
  return data;
}

/* Reads CNT 16-bit (halfword) units from PORT, one after
   another, and stores them into the buffer starting at ADDR. */
static inline void
insw (uint16_t port, void *addr, size_t cnt)
{
  
  asm volatile ("rep insw" : "+D" (addr), "+c" (cnt) : "d" (port) : "memory");
}


static inline uint32_t
inl (uint16_t port)
{
  
  uint32_t data;
  asm volatile ("inl %w1, %0" : "=a" (data) : "Nd" (port));
  return data;
}

/* Reads CNT 32-bit (word) units from PORT, one after another,
   and stores them into the buffer starting at ADDR. */
static inline void
insl (uint16_t port, void *addr, size_t cnt)
{
  
  asm volatile ("rep insl" : "+D" (addr), "+c" (cnt) : "d" (port) : "memory");
}


static inline void
outb (uint16_t port, uint8_t data)
{
  
  asm volatile ("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

/* Writes to PORT each byte of data in the CNT-byte buffer
   starting at ADDR. */
static inline void
outsb (uint16_t port, const void *addr, size_t cnt)
{
  
  asm volatile ("rep outsb" : "+S" (addr), "+c" (cnt) : "d" (port));
}


static inline void
outw (uint16_t port, uint16_t data)
{
  
  asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

/* Writes to PORT each 16-bit unit (halfword) of data in the
   CNT-halfword buffer starting at ADDR. */
static inline void
outsw (uint16_t port, const void *addr, size_t cnt)
{
  
  asm volatile ("rep outsw" : "+S" (addr), "+c" (cnt) : "d" (port));
}


static inline void
outl (uint16_t port, uint32_t data)
{
  
  asm volatile ("outl %0, %w1" : : "a" (data), "Nd" (port));
}

/* Writes to PORT each 32-bit unit (word) of data in the CNT-word
   buffer starting at ADDR. */
static inline void
outsl (uint16_t port, const void *addr, size_t cnt)
{
  
  asm volatile ("rep outsl" : "+S" (addr), "+c" (cnt) : "d" (port));
}

#endif 
