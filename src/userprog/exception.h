#ifndef USERPROG_EXCEPTION_H
#define USERPROG_EXCEPTION_H


#define PF_P 0x1    
#define PF_W 0x2    
#define PF_U 0x4    

void exception_init (void);
void exception_print_stats (void);

#endif 
