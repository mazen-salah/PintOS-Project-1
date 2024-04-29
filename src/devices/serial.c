#include "devices/serial.h"
#include <debug.h>
#include "devices/input.h"
#include "devices/intq.h"
#include "devices/timer.h"
#include "threads/io.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "threads/thread.h"

/* Register definitions for the 16550A UART used in PCs.
   The 16550A has a lot more going on than shown here, but this
   is all we need.

   Refer to [PC16650D] for hardware information. */


#define IO_BASE 0x3f8


#define RBR_REG (IO_BASE + 0)   
#define THR_REG (IO_BASE + 0)   
#define IER_REG (IO_BASE + 1)   


#define LS_REG (IO_BASE + 0)    
#define MS_REG (IO_BASE + 1)    


#define IIR_REG (IO_BASE + 2)   
#define FCR_REG (IO_BASE + 2)   
#define LCR_REG (IO_BASE + 3)   
#define MCR_REG (IO_BASE + 4)   
#define LSR_REG (IO_BASE + 5)   


#define IER_RECV 0x01           
#define IER_XMIT 0x02           


#define LCR_N81 0x03            
#define LCR_DLAB 0x80           


#define MCR_OUT2 0x08           


#define LSR_DR 0x01             
#define LSR_THRE 0x20           


static enum { UNINIT, POLL, QUEUE } mode;


static struct intq txq;

static void set_serial (int bps);
static void putc_poll (uint8_t);
static void write_ier (void);
static intr_handler_func serial_interrupt;

/* Initializes the serial port device for polling mode.
   Polling mode busy-waits for the serial port to become free
   before writing to it.  It's slow, but until interrupts have
   been initialized it's all we can do. */
static void
init_poll (void) 
{
  ASSERT (mode == UNINIT);
  outb (IER_REG, 0);                    
  outb (FCR_REG, 0);                    
  set_serial (9600);                    
  outb (MCR_REG, MCR_OUT2);             
  intq_init (&txq);
  mode = POLL;
} 

/* Initializes the serial port device for queued interrupt-driven
   I/O.  With interrupt-driven I/O we don't waste CPU time
   waiting for the serial device to become ready. */
void
serial_init_queue (void) 
{
  enum intr_level old_level;

  if (mode == UNINIT)
    init_poll ();
  ASSERT (mode == POLL);

  intr_register_ext (0x20 + 4, serial_interrupt, "serial");
  mode = QUEUE;
  old_level = intr_disable ();
  write_ier ();
  intr_set_level (old_level);
}


void
serial_putc (uint8_t byte) 
{
  enum intr_level old_level = intr_disable ();

  if (mode != QUEUE)
    {
      /* If we're not set up for interrupt-driven I/O yet,
         use dumb polling to transmit a byte. */
      if (mode == UNINIT)
        init_poll ();
      putc_poll (byte); 
    }
  else 
    {
      /* Otherwise, queue a byte and update the interrupt enable
         register. */
      if (old_level == INTR_OFF && intq_full (&txq)) 
        {
          /* Interrupts are off and the transmit queue is full.
             If we wanted to wait for the queue to empty,
             we'd have to reenable interrupts.
             That's impolite, so we'll send a character via
             polling instead. */
          putc_poll (intq_getc (&txq)); 
        }

      intq_putc (&txq, byte); 
      write_ier ();
    }
  
  intr_set_level (old_level);
}

/* Flushes anything in the serial buffer out the port in polling
   mode. */
void
serial_flush (void) 
{
  enum intr_level old_level = intr_disable ();
  while (!intq_empty (&txq))
    putc_poll (intq_getc (&txq));
  intr_set_level (old_level);
}

/* The fullness of the input buffer may have changed.  Reassess
   whether we should block receive interrupts.
   Called by the input buffer routines when characters are added
   to or removed from the buffer. */
void
serial_notify (void) 
{
  ASSERT (intr_get_level () == INTR_OFF);
  if (mode == QUEUE)
    write_ier ();
}


static void
set_serial (int bps)
{
  int base_rate = 1843200 / 16;         
  uint16_t divisor = base_rate / bps;   

  ASSERT (bps >= 300 && bps <= 115200);

  
  outb (LCR_REG, LCR_N81 | LCR_DLAB);

  
  outb (LS_REG, divisor & 0xff);
  outb (MS_REG, divisor >> 8);
  
  
  outb (LCR_REG, LCR_N81);
}


static void
write_ier (void) 
{
  uint8_t ier = 0;

  ASSERT (intr_get_level () == INTR_OFF);

  /* Enable transmit interrupt if we have any characters to
     transmit. */
  if (!intq_empty (&txq))
    ier |= IER_XMIT;

  /* Enable receive interrupt if we have room to store any
     characters we receive. */
  if (!input_full ())
    ier |= IER_RECV;
  
  outb (IER_REG, ier);
}

/* Polls the serial port until it's ready,
   and then transmits BYTE. */
static void
putc_poll (uint8_t byte) 
{
  ASSERT (intr_get_level () == INTR_OFF);

  while ((inb (LSR_REG) & LSR_THRE) == 0)
    continue;
  outb (THR_REG, byte);
}


static void
serial_interrupt (struct intr_frame *f UNUSED) 
{
  /* Inquire about interrupt in UART.  Without this, we can
     occasionally miss an interrupt running under QEMU. */
  inb (IIR_REG);

  /* As long as we have room to receive a byte, and the hardware
     has a byte for us, receive a byte.  */
  while (!input_full () && (inb (LSR_REG) & LSR_DR) != 0)
    input_putc (inb (RBR_REG));

  /* As long as we have a byte to transmit, and the hardware is
     ready to accept a byte for transmission, transmit a byte. */
  while (!intq_empty (&txq) && (inb (LSR_REG) & LSR_THRE) != 0) 
    outb (THR_REG, intq_getc (&txq));

  
  write_ier ();
}
