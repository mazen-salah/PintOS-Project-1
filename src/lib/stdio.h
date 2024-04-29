#ifndef __LIB_STDIO_H
#define __LIB_STDIO_H

#include <debug.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Include lib/user/stdio.h or lib/kernel/stdio.h, as
   appropriate. */
#include_next <stdio.h>


#define STDIN_FILENO 0
#define STDOUT_FILENO 1


int printf (const char *, ...) PRINTF_FORMAT (1, 2);
int snprintf (char *, size_t, const char *, ...) PRINTF_FORMAT (3, 4);
int vprintf (const char *, va_list) PRINTF_FORMAT (1, 0);
int vsnprintf (char *, size_t, const char *, va_list) PRINTF_FORMAT (3, 0);
int putchar (int);
int puts (const char *);


void hex_dump (uintptr_t ofs, const void *, size_t size, bool ascii);
void print_human_readable_size (uint64_t sz);


void __vprintf (const char *format, va_list args,
                void (*output) (char, void *), void *aux);
void __printf (const char *format,
               void (*output) (char, void *), void *aux, ...);


#define sprintf dont_use_sprintf_use_snprintf
#define vsprintf dont_use_vsprintf_use_vsnprintf

#endif 
