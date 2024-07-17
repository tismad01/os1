#include "../inc/mem.hpp"
#include "../inc/list.hpp"


List<size_t> *used_list;
const size_t MEM_NODE_SIZE = MEM_BLOCK_SIZE > sizeof(List<size_t>) ? MEM_BLOCK_SIZE : sizeof(List<size_t>);

uint64 align(uint64 n, uint64 mod = MEM_BLOCK_SIZE) {
    if (n % mod == 0)
        return n;
    return n + mod - (n % mod);
}

void* mem_kalloc(size_t size) {
	size_t required = MEM_NODE_SIZE + align(size);
    for (auto cur = used_list; cur != used_list->prev; cur = cur->next) {
        if ((size_t)(void*)cur->next - (size_t)(void*)cur - cur->data >= required) {
            List<size_t> *new_node = (List<size_t>*)(void*)((size_t)(void*)cur + cur->data);
            new_node->data = required;
            cur->next->listAdd(new_node);
			void *ret = (void*)((uint64)(void*)new_node + MEM_NODE_SIZE);
			for (uint64 i = 0; i < size; i++)
				((char*)ret)[i] = 0;

			return ret;
        }
    }
	return nullptr;
}

void* mem_alloc(size_t size) {
    return mem_kalloc(size * MEM_BLOCK_SIZE);
}

int mem_free(void* ptr) {
    if (used_list != nullptr) {
        for (auto cur = used_list->next; cur != used_list->prev; cur = cur->next) {
            if (MEM_NODE_SIZE + (size_t)(void*)cur == (size_t)ptr) {
                cur->next->listRemove();
                return 0;
            }
        }
    }
    return -EINVALID_FREE_PTR;
}

int mem_init() {
    auto aux_first  = (List<size_t>*)align((uint64)HEAP_START_ADDR, MEM_NODE_SIZE);
    auto aux_second = (List<size_t>*)(void*) ((uint64)HEAP_END_ADDR - MEM_NODE_SIZE);
    aux_first->data = MEM_NODE_SIZE;

    aux_second->data = MEM_NODE_SIZE;

    aux_first->next = aux_first->prev = aux_second;
    aux_second->next = aux_second->prev = aux_first;

    used_list = aux_first;
    return 0;
}



