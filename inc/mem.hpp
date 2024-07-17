#ifndef MEM_H
#define MEM_H
#include "../lib/hw.h"

#define EINVALID_FREE_PTR 1

void* mem_alloc(size_t size);
void *mem_kalloc(size_t size);
int mem_free(void* ptr);
int mem_init();

#endif
