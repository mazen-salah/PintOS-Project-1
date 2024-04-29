#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <syscall-nr.h>

/* The standard vprintf() function,
   which is like printf() but uses a va_list. */
int
vprintf (const char *format, va_list args) 
{
  return vhprintf (STDOUT_FILENO, format, args);
}


int
hprintf (int handle, const char *format, ...) 
{
  va_list args;
  int retval;

  va_start (args, format);
  retval = vhprintf (handle, format, args);
  va_end (args);

  return retval;
}

/* Writes string S to the console, followed by a new-line
   character. */
int
puts (const char *s) 
{
  write (STDOUT_FILENO, s, strlen (s));
  putchar ('\n');

  return 0;
}


int
putchar (int c) 
{
  char c2 = c;
  write (STDOUT_FILENO, &c2, 1);
  return c;
}


struct vhprintf_aux 
  {
    char buf[64];       
    char *p;            
    int char_cnt;       
    int handle;         
  };

static void add_char (char, void *);
static void flush (struct vhprintf_aux *);

/* Formats the printf() format specification FORMAT with
   arguments given in ARGS and writes the output to the given
   HANDLE. */
int
vhprintf (int handle, const char *format, va_list args) 
{
  struct vhprintf_aux aux;
  aux.p = aux.buf;
  aux.char_cnt = 0;
  aux.handle = handle;
  __vprintf (format, args, add_char, &aux);
  flush (&aux);
  return aux.char_cnt;
}

/* Adds C to the buffer in AUX, flushing it if the buffer fills
   up. */
static void
add_char (char c, void *aux_) 
{
  struct vhprintf_aux *aux = aux_;
  *aux->p++ = c;
  if (aux->p >= aux->buf + sizeof aux->buf)
    flush (aux);
  aux->char_cnt++;
}


static void
flush (struct vhprintf_aux *aux)
{
  if (aux->p > aux->buf)
    write (aux->handle, aux->buf, aux->p - aux->buf);
  aux->p = aux->buf;
}
