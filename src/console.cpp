#include "../inc/console.hpp"
#include "../inc/thread.hpp"
#include "../inc/sem.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

void putc(char x) {
    __putc(x);
}

char getc() {
    return __getc();
}
