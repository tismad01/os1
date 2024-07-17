#include "../inc/syscall_cpp.hpp"

void* operator new(size_t sz) {
    return mem_alloc(sz);
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

Thread::Thread (void (*body)(void*), void* arg) {
    thread_create(&myHandle,body, arg);
}

Thread::~Thread() {

}

void Thread::_run(void* _thread){
    auto thread = (Thread*)_thread;
    thread->run();
}

void Thread::start() {
    if (body == nullptr)
        thread_create(&myHandle, _run, (void*)this);
    else
        thread_create(&myHandle, body, arg);
}

void Thread::join() {
    thread_join((thread_t)&arg);
}

void Thread::dispatch(){
    thread_dispatch();
}
void Thread::sleep(time_t t){
    time_sleep(t);
}

Thread::Thread() {

}

Semaphore::Semaphore(unsigned init){
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {

}

void Semaphore::wait(){
    sem_wait(myHandle);
}

void Semaphore::signal(){
    sem_signal(myHandle);
}

PeriodicThread::PeriodicThread (time_t period) {

}

char Console::getc(){
    return ::getc();
}
void Console::putc(char c){
    ::putc(c);
}
