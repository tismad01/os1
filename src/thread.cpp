#include "../inc/thread.hpp"
#include "../inc/mem.hpp"
#include "../inc/util.h"
#include "../inc/config.h"

List<_thread> *active_threads = nullptr;

void _tinit(uint64 ph, void(*start_routine)(void*), void* arg) {
    start_routine(arg);

    write_reg(a0, SYSCALL_THREAD_EXIT);
    asm volatile("ecall");
}

void thread_populate(thread_t handle, void (*start_routine)(void*), void* arg, void* stack) {
    auto sp = (uint64)stack + DEFAULT_STACK_SIZE - 512 - 16;
    handle->pc = (uint64)_tinit;
    handle->sp = sp;
    handle->stack = (uint64)stack;
    handle->a0 = 0;
    handle->waiting = nullptr;

    auto regs = (uint64*) sp;
    regs[17] = (uint64)start_routine;
    regs[18] = (uint64)arg;

}

void thread_append(List<_thread> *t) {
    if (active_threads)
        active_threads->listAdd(t);
    else {
        active_threads = t;
        t->next = t->prev = t;
    }
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack) {
    List<_thread> *new_thread = (List<_thread>*)mem_kalloc(sizeof(List<_thread>));
    if (new_thread == nullptr)
        return -1;
    thread_populate(&(new_thread->data), start_routine, arg, stack);

    thread_append(new_thread);
    *handle = &new_thread->data;
    return 0;
}

void load_context(thread_t t) {
    write_sreg(sepc, t->pc);
    write_sreg(sscratch, t->sp);
}

void store_context(thread_t t) {
    read_sreg(sepc, t->pc);
    read_sreg(sscratch, t->sp);
}

thread_t aux_thread;

uint64 thread_load_next() {
    auto curr = active_threads ? &(active_threads->data) : aux_thread;
    load_context(curr);
    return curr->a0;
}

void thread_set_a0(uint64 a0) {
    if (active_threads)
        active_threads->data.a0 = a0;
}

uint64 thread_get_a0() {
    if (active_threads)
        return active_threads->data.a0;
    return aux_thread->a0;
}
void aux_thread_fn(void* arg) {
    while(1) {
        asm volatile("wfi");
    }
}

int thread_init() {
	active_threads = nullptr;
    auto stack = mem_kalloc(DEFAULT_STACK_SIZE);
    aux_thread = (thread_t)mem_kalloc(sizeof(_thread));
    thread_populate(aux_thread, aux_thread_fn, nullptr, stack);
    return 0;
}

void pop_front() {
    active_threads = active_threads->next;
    if (active_threads->listRemove() == nullptr){
        active_threads = nullptr;
    }
}

void thread_dispatch() {
    if(active_threads != active_threads->next) {
        store_context(&(active_threads->data));

        active_threads = active_threads->next;

        load_context(&(active_threads->data));
    }
}

void thread_join(thread_t handle) {
    auto curr = active_threads;
    store_context(&(curr->data));
    pop_front();
    if (handle->waiting != nullptr)
        handle->waiting->listAdd(curr);
    else {
        curr->next = curr->prev = curr;
        handle->waiting = curr;
    }

    load_context(&(active_threads->data));
}

int thread_exit() {
    List<_thread> *curr = active_threads;
    pop_front();

    if (curr->data.waiting) {
        List<_thread> *removed;
        while ((removed = curr->data.waiting->listRemove()) != nullptr) {
            if (active_threads)
                active_threads->listAdd(removed);
            else {
                active_threads = removed;
                removed->prev = removed->next = removed;
            }
        }
        removed = curr->data.waiting;
        if (active_threads) {
            active_threads->listAdd(removed);
        }
        else {
            active_threads = removed;
            removed->prev = removed->next = removed;
        }
    }
    mem_free((void*)curr->data.stack);
	mem_free((void*)curr);

    thread_load_next();
    return 0;
}

int time_sleep(time_t t) {
    return 0;
}

void thread_save_context(uint64 a0) {
    if (active_threads) {
        store_context(&(active_threads->data));
        active_threads->data.a0 = a0;
    }
    else {
        store_context(aux_thread);
        aux_thread->a0 = a0;
    }
}