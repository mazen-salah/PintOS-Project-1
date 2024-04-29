#ifndef __LIB_SYSCALL_NR_H
#define __LIB_SYSCALL_NR_H


enum 
  {
    
    SYS_HALT,                   
    SYS_EXIT,                   
    SYS_EXEC,                   
    SYS_WAIT,                   
    SYS_CREATE,                 
    SYS_REMOVE,                 
    SYS_OPEN,                   
    SYS_FILESIZE,               
    SYS_READ,                   
    SYS_WRITE,                  
    SYS_SEEK,                   
    SYS_TELL,                   
    SYS_CLOSE,                  

    
    SYS_MMAP,                   
    SYS_MUNMAP,                 

    
    SYS_CHDIR,                  
    SYS_MKDIR,                  
    SYS_READDIR,                
    SYS_ISDIR,                  
    SYS_INUMBER                 
  };

#endif 
