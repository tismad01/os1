#ifndef CONFIG_H
#define CONFIG_H

#define SYSCALL_MEM_ALLOC 0x01
#define SYSCALL_MEM_FREE 0x02
#define SYSCALL_THREAD_CREATE 0x11
#define SYSCALL_THREAD_EXIT 0x12
#define SYSCALL_THREAD_DISPATCH 0x13
#define SYSCALL_THREAD_JOIN 0x14
#define SYSCALL_SEM_OPEN 0x21
#define SYSCALL_SEM_CLOSE 0x22
#define SYSCALL_SEM_WAIT 0x23
#define SYSCALL_SEM_SIGNAL 0x24
#define SYSCALL_TIME_SLEEP 0x31
#define SYSCALL_GETC 0x41
#define SYSCALL_PUTC 0x42

#endif
