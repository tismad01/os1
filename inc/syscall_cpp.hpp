#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.h"

void* operator new(size_t);
void operator delete(void*);

class Thread{
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread();

    void start();

    void join();

    static void dispatch();
    static void sleep(time_t);

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    static void _run(void*);
};

class Semaphore{
public:

    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    void wait();
    void signal();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread{
public:
    void terminate();
protected:
    friend void pfn(void* _f);
    PeriodicThread (time_t period);
    virtual void periodicActivation() {}
private:
    time_t period;
};

class Console{
public:
    static char getc();
    static void putc(char);
};
#endif