
#include "../inc/mem.hpp"
#include "../inc/thread.hpp"
#include "../inc/util.h"
#include "../inc/config.h"
#include "../inc/sem.hpp"
#include "../inc/console.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

#define ILLEGAL_INST 2
#define ILLEGAL_READ 5
#define ILLEGAL_WRITE 7
#define USER_ECALL 8
#define SYSTEM_ECALL 9
#define SYS_INT_HANDLE() \
        read_sreg(sip, sip); \
        SET0(sip, 1); \
        write_sreg(sip,sip)

void panic(const char *msg = "[panic] ??"){
    while(1) {
        asm volatile("wfi");
    }
}

void userMain();

extern "C" void interruptvec();

extern "C" uint64 main_hw(uint64 a0) {
    uint64 scause;
    read_sreg(scause, scause);

    console_handler();
    a0 = thread_get_a0();
    return a0;
}

extern "C" uint64 main_timer(uint64 a0) {
    uint64 sip;
    SYS_INT_HANDLE();
    return a0;
}

extern "C" uint64 main_interrupt(uint64 a0, uint64 a1, uint64 a2, uint64 a3){
    void *stack;
    uint64 blocks;
    uint64 scause, sstatus, sip, sepc;
    read_sreg(scause, scause);

    if(scause == ILLEGAL_INST) {
		panic("bad instr");
    }
    else if(scause == ILLEGAL_READ) {
        panic("bad read");
    }
    else if(scause == ILLEGAL_WRITE) {
        panic("bad write");
    }
    else if(scause == USER_ECALL) {
        read_sreg(sepc, sepc);
        sepc+=4;
        write_sreg(sepc, sepc);

        switch (a0) {
            case (SYSCALL_MEM_ALLOC):
                blocks = (a1 + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
                a0 = (uint64)mem_alloc(blocks);
                break;
            case (SYSCALL_MEM_FREE):
                a0 = (uint64)mem_free((void*)a1);
                break;
            case (SYSCALL_THREAD_CREATE):
                stack = mem_kalloc(DEFAULT_STACK_SIZE);
                a0 = (uint64)thread_create((thread_t*)a1, (void(*)(void*))a2, (void*)a3, stack);
                break;
            case (SYSCALL_THREAD_EXIT):
                a0 = (uint64)thread_exit();
                a0 = thread_get_a0();
                break;
			case (SYSCALL_THREAD_JOIN):
				thread_join((thread_t)a1);
				break;
            case (SYSCALL_THREAD_DISPATCH):
                thread_set_a0(a0);
                thread_dispatch();
                a0 = thread_get_a0();
                break;
            case (SYSCALL_SEM_OPEN):
                a0 = (uint64)sem_open((sem_t*)a1,(unsigned)a2);
                break;
            case (SYSCALL_SEM_CLOSE):
                a0 = (uint64)sem_close((sem_t)a1);
                break;
            case (SYSCALL_SEM_WAIT):
				thread_set_a0(0);
                a0 = (uint64)sem_wait((sem_t)a1);
				a0 = thread_get_a0();
                break;
            case (SYSCALL_SEM_SIGNAL):
                a0 = (uint64)sem_signal((sem_t)a1);
                break;
            case (SYSCALL_TIME_SLEEP):
                a0 = (uint64)time_sleep((time_t)a1);
                break;
            case (SYSCALL_GETC):
				store_context(&active_threads->data);
                a0 = getc();
				load_context(&active_threads->data);
                break;
            case (SYSCALL_PUTC):
				store_context(&active_threads->data);
		        putc((char)a1);
				load_context(&active_threads->data);
                break;
            default:
                panic("unknown syscall");
        }

        SYS_INT_HANDLE();
    }
    else if (scause == SYSTEM_ECALL) {
        read_sreg(sstatus, sstatus);
        SET0(sstatus, 8);
        write_sreg(sstatus, sstatus);

        thread_t handle;
        stack = mem_kalloc(DEFAULT_STACK_SIZE);

        thread_create(&handle, (void(*)(void*))userMain, nullptr, stack);

        write_sreg(sepc, _tinit);
        write_sreg(sscratch, handle->sp);

        SYS_INT_HANDLE();
    }
    else {
        panic("unknown scause");
	}
    return a0;
}


void main() {
    uint64 x = 1 + (uint64)&interruptvec;
    write_sreg(stvec, x);
    mem_init();
    thread_init();
    asm volatile ("ecall");
}
