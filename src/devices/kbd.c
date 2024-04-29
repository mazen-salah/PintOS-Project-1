#include "devices/kbd.h"
#include <ctype.h>
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include "devices/input.h"
#include "devices/shutdown.h"
#include "threads/interrupt.h"
#include "threads/io.h"


#define DATA_REG 0x60

/* Current state of shift keys.
   True if depressed, false otherwise. */
static bool left_shift, right_shift;    
static bool left_alt, right_alt;        
static bool left_ctrl, right_ctrl;      

/* Status of Caps Lock.
   True when on, false when off. */
static bool caps_lock;


static int64_t key_cnt;

static intr_handler_func keyboard_interrupt;


void
kbd_init (void) 
{
  intr_register_ext (0x21, keyboard_interrupt, "8042 Keyboard");
}


void
kbd_print_stats (void) 
{
  printf ("Keyboard: %lld keys pressed\n", key_cnt);
}


struct keymap
  {
    uint8_t first_scancode;     
    const char *chars;          /* chars[0] has scancode first_scancode,
                                   chars[1] has scancode first_scancode + 1,
                                   and so on to the end of the string. */
  };
  
/* Keys that produce the same characters regardless of whether
   the Shift keys are down.  Case of letters is an exception
   that we handle elsewhere.  */
static const struct keymap invariant_keymap[] = 
  {
    {0x01, "\033"},             
    {0x0e, "\b"},
    {0x0f, "\tQWERTYUIOP"},
    {0x1c, "\r"},
    {0x1e, "ASDFGHJKL"},
    {0x2c, "ZXCVBNM"},
    {0x37, "*"},
    {0x39, " "},
    {0x53, "\177"},             
    {0, NULL},
  };

/* Characters for keys pressed without Shift, for those keys
   where it matters. */
static const struct keymap unshifted_keymap[] = 
  {
    {0x02, "1234567890-="},
    {0x1a, "[]"},
    {0x27, ";'`"},
    {0x2b, "\\"},
    {0x33, ",./"},
    {0, NULL},
  };
  
/* Characters for keys pressed with Shift, for those keys where
   it matters. */
static const struct keymap shifted_keymap[] = 
  {
    {0x02, "!@#$%^&*()_+"},
    {0x1a, "{}"},
    {0x27, ":\"~"},
    {0x2b, "|"},
    {0x33, "<>?"},
    {0, NULL},
  };

static bool map_key (const struct keymap[], unsigned scancode, uint8_t *);

static void
keyboard_interrupt (struct intr_frame *args UNUSED) 
{
  
  bool shift = left_shift || right_shift;
  bool alt = left_alt || right_alt;
  bool ctrl = left_ctrl || right_ctrl;

  
  unsigned code;

  
  bool release;

  
  uint8_t c;

  
  code = inb (DATA_REG);
  if (code == 0xe0)
    code = (code << 8) | inb (DATA_REG);

  /* Bit 0x80 distinguishes key press from key release
     (even if there's a prefix). */
  release = (code & 0x80) != 0;
  code &= ~0x80u;

  
  if (code == 0x3a) 
    {
      
      if (!release)
        caps_lock = !caps_lock;
    }
  else if (map_key (invariant_keymap, code, &c)
           || (!shift && map_key (unshifted_keymap, code, &c))
           || (shift && map_key (shifted_keymap, code, &c)))
    {
      
      if (!release) 
        {
          
          if (c == 0177 && ctrl && alt)
            shutdown_reboot ();

          /* Handle Ctrl, Shift.
             Note that Ctrl overrides Shift. */
          if (ctrl && c >= 0x40 && c < 0x60) 
            {
              
              c -= 0x40; 
            }
          else if (shift == caps_lock)
            c = tolower (c);

          /* Handle Alt by setting the high bit.
             This 0x80 is unrelated to the one used to
             distinguish key press from key release. */
          if (alt)
            c += 0x80;

          
          if (!input_full ())
            {
              key_cnt++;
              input_putc (c);
            }
        }
    }
  else
    {
      
      struct shift_key 
        {
          unsigned scancode;
          bool *state_var;
        };

      
      static const struct shift_key shift_keys[] = 
        {
          {  0x2a, &left_shift},
          {  0x36, &right_shift},
          {  0x38, &left_alt},
          {0xe038, &right_alt},
          {  0x1d, &left_ctrl},
          {0xe01d, &right_ctrl},
          {0,      NULL},
        };
  
      const struct shift_key *key;

      
      for (key = shift_keys; key->scancode != 0; key++) 
        if (key->scancode == code)
          {
            *key->state_var = !release;
            break;
          }
    }
}

/* Scans the array of keymaps K for SCANCODE.
   If found, sets *C to the corresponding character and returns
   true.
   If not found, returns false and C is ignored. */
static bool
map_key (const struct keymap k[], unsigned scancode, uint8_t *c) 
{
  for (; k->first_scancode != 0; k++)
    if (scancode >= k->first_scancode
        && scancode < k->first_scancode + strlen (k->chars)) 
      {
        *c = k->chars[scancode - k->first_scancode];
        return true; 
      }

  return false;
}
