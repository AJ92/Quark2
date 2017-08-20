#include "poolloc.h"


Poolloc::Poolloc() {
    std::cout << "sizeof(void *) = " << sizeof(void *) << std::endl;
    std::cout << "sizeof(std::uintptr_t) = " << sizeof(std::uintptr_t) << std::endl;
    std::cout << "sizeof(std::size_t) = " << sizeof(std::size_t) << std::endl;

    _alloc_size = 64;

    _pool_size = 1024;
    _mem_pool_pos = 0;
    _mem_pool_size = sizeof(void *) * _pool_size;

    void * mem = malloc(_mem_pool_size);
    memset(mem, 0, _mem_pool_size);

    _mem_pool = (std::uintptr_t *)mem;
}

Poolloc::~Poolloc() {
    //clean up used mem...
    //TODO...

    _mem_pool -= _mem_pool_pos;
    free(_mem_pool);
}

void * Poolloc::allocateNew(std::size_t s){
    if(s > _alloc_size){
        //allocate new memory
        void * mem = malloc(s);
        //std::cout << "alloc @  " << std::hex << mem << " size = " << std::dec  << s << std::endl;
        return mem;
    }
    if(_mem_pool_pos == 0)
    {
        //allocate new memory
        void * mem = malloc(_alloc_size);
        //std::cout << "alloc @  " << std::hex << mem << " size = " << std::dec  << s << std::endl;
        return mem;
    }

    _mem_pool_pos -= 1;
    _mem_pool -= 1;

    void * element_ptr;
    memcpy(&element_ptr, (void *)_mem_pool, sizeof(void *));

    //std::cout << "poolloc @  " << std::hex << element_ptr << " size = " << std::dec << s << std::endl;

    return element_ptr;
}
void Poolloc::allocateFree(void* p){
    //pool is full, free the mem
    if(_mem_pool_pos >= _pool_size){
        free(p);
        return;
    }

    //else add the mem to the pool...

    memcpy((void *)_mem_pool, &p, sizeof(void *));
    //std::cout << "poolloc free @  " << std::hex << p << std::endl;
    _mem_pool += 1;

    _mem_pool_pos+=1;
}