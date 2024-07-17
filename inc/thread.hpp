#ifndef THREAD_H
#define THREAD_H
#include "../lib/hw.h"
#include "list.hpp"

struct _thread {
    uint64 pc;
    uint64 sp;
    uint64 stack;
    uint64 a0;
    List<_thread> *waiting;
};
typedef struct _thread* thread_t;
extern List<_thread> *active_threads;

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg, void* stack);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t handle);

uint64 thread_load_next();
void thread_set_a0(uint64 a0);
uint64 thread_get_a0();
void _tinit(uint64 ph, void(*start_routine)(void*), void* arg);
void thread_append(List<_thread> *t);
int thread_init();
void thread_save_context(uint64 a0);
void pop_front();

int time_sleep(time_t);
void load_context(thread_t t);

void store_context(thread_t t);
#endif
