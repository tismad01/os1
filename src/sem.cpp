#include "../inc/sem.hpp"
#include "../inc/thread.hpp"
#include "../inc/mem.hpp"
#include "../inc/util.h"


typedef struct _sem *sem_t;

int sem_open(sem_t *handle, unsigned init) {
    auto ret = (sem_t)mem_kalloc(sizeof(_sem));
    if (ret == nullptr) {
        return -1;
    }
    ret->cnt = init;
    ret->waiting = nullptr;
    *handle = ret;

    return 0;
}

int sem_close(sem_t handle) {
    if (!handle)
        return -2;
    if (handle->waiting) {
        List<_thread> *curr;
        while ((curr = handle->waiting->listRemove()) != nullptr) {
            active_threads->listAdd(curr);
            curr->data.a0 = -1;
            mem_free(curr);
        }

        active_threads->listAdd(handle->waiting);
        handle->waiting->data.a0 = -1;
        mem_free(handle->waiting);
    }

    return mem_free(handle);
}
int sem_wait(sem_t id) {
    if (!id) return -2;
    if (id->cnt > 0) {
        id->cnt--;
    }
    else {
        thread_save_context(0);
        auto curr = active_threads;
        active_threads = active_threads->next;
        if (active_threads->listRemove() == nullptr) {
            active_threads = nullptr;
        }
        if (id->waiting == nullptr) {
            id->waiting = curr;
            curr->next = curr->prev = curr;
        }
        else
            id->waiting->listAdd(curr);

        thread_load_next();
    }
    return 0;
}

int sem_signal(sem_t id) {
    if (!id)
        return -2;
    if (id->waiting != nullptr) {
        auto curr = id->waiting;
        if (id->waiting->next != id->waiting) {
            id->waiting = id->waiting->next;
            id->waiting->listRemove();
        }
        else {
            id->waiting = nullptr;
        }
        thread_append(curr);
    }
    else {
        id->cnt++;
    }

    return 0;
}
