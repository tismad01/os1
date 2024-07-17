#ifndef SEM_H
#define SEM_H
#include "thread.hpp"

struct _sem{
    unsigned cnt;
    List<_thread> *waiting;
};
typedef struct _sem *sem_t;

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);


#endif