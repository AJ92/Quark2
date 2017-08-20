//CUSTOM NEW OPERATORS FOR DEFAULT MEMORY ALLOCS

#ifndef MEMMANAGEMENT_H
#define MEMMANAGEMENT_H

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <new>

#include "base/mem/allocator/poolloc.h"

//custom memory allocator
static Poolloc allocator;

//custom operator new and delete

static std::size_t memory_used = 0;
static std::size_t alloc_used = 0;

void* operator new(std::size_t s) throw(std::bad_alloc)
{
    memory_used += s;
    ++alloc_used;
    //return malloc(s);
    return allocator.allocateNew(s);
}

void  operator delete(void* p) throw()
{
    --alloc_used;
    //free(p);
    allocator.allocateFree(p);
}

void memuse()
{
    std::cout << "memory = " << memory_used <<
    "  alloc = " << alloc_used << std::endl;
}


#endif //MEMMANAGEMENT_H