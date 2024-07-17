#ifndef _LIST_HPP_
#define _LIST_HPP_

template <typename T>
struct List {
    List *next, *prev;
    T data;

    List(T data) {
        this->data = data;
        this->next = this->prev = this;
    }

    void listAdd(List *l) {
        l->prev = this->prev;
        l->next = this;
        this->prev->next = l;
        this->prev = l;
    }

    List* listRemove() {
        if (this == this->prev) {
            return nullptr;
        }
        List* removed = this->prev;
        this->prev->prev->next = this;
        this->prev = this->prev->prev;
        return removed;
    }
};

#endif