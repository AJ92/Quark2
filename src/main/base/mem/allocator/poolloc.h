#ifndef POOLLOC_H
#define POOLLOC_H

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <iostream>
#include <new>


class Poolloc
{
public:
    Poolloc();
	~Poolloc();

    void * allocateNew(std::size_t s);
    void allocateFree(void* p);

private:  
    std::size_t _alloc_size;

    std::size_t _pool_size;
    std::size_t _mem_pool_pos;
    std::size_t _mem_pool_size;
    std::uintptr_t * _mem_pool;

};


#endif //POOLLOC_H