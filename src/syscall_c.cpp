#include "../inc/syscall_c.h"
#include "../inc/util.h"
#include "../inc/config.h"

uint64 _syscall(uint64 code, uint64 arg1, uint64 arg2, uint64 arg3) {
    uint64 ret;
    write_reg(a0, code);
    write_reg(a1, arg1);
    write_reg(a2, arg2);
    write_reg(a3, arg3);
    asm volatile("ecall");
    read_reg(a0, ret);
    return ret;
}
#define syscall(code, arg1, arg2, arg3) \
        _syscall((uint64)code, (uint64)arg1, (uint64)arg2,(uint64)arg3)

void *mem_alloc(size_t size) {
    return (void*)syscall(SYSCALL_MEM_ALLOC, size, 0, 0);
}
int mem_free(void *x) {
    return (int)syscall(SYSCALL_MEM_FREE, x, 0, 0);
}

int thread_create(thread_t *handle, void (*start_routine)(void*), void *arg) {
    return (int)syscall(SYSCALL_THREAD_CREATE, handle, start_routine, arg);
}
int thread_exit() {
    return (int)syscall(SYSCALL_THREAD_EXIT, 0,0,0);
}
void thread_dispatch() {
    return (void)syscall(SYSCALL_THREAD_DISPATCH, 0,0,0);
}

void thread_join(thread_t handle) {
    return (void)syscall(SYSCALL_THREAD_JOIN, handle, 0, 0);
}

int sem_open(sem_t* handle, unsigned init) {
    return (int)syscall(SYSCALL_SEM_OPEN, handle, init, 0);
}
int sem_close(sem_t handle) {
    return (int)syscall(SYSCALL_SEM_CLOSE, handle,0,0);
}
int sem_wait(sem_t id) {
    return (int)syscall(SYSCALL_SEM_WAIT, id,0,0);
}
int sem_signal(sem_t id) {
    return (int)syscall(SYSCALL_SEM_SIGNAL, id,0,0);
}

int time_sleep(time_t t) {
    return (int)syscall(SYSCALL_TIME_SLEEP, t, 0,0);
}

char getc() {
    return (char)syscall(SYSCALL_GETC, 0,0,0);
}
void putc(char c) {
    return (void)syscall(SYSCALL_PUTC, c,0,0);
}