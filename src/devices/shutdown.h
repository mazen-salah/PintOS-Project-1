#ifndef DEVICES_SHUTDOWN_H
#define DEVICES_SHUTDOWN_H

#include <debug.h>


enum shutdown_type
  {
    SHUTDOWN_NONE,              
    SHUTDOWN_POWER_OFF,         
    SHUTDOWN_REBOOT,            
  };

void shutdown (void);
void shutdown_configure (enum shutdown_type);
void shutdown_reboot (void) NO_RETURN;
void shutdown_power_off (void) NO_RETURN;

#endif 
