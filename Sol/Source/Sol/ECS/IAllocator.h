#pragma once
#include <cstdint>

namespace Sol 
{
    //TODO eventually we want to allocate a chunk of heap memory as the applicaiton starts,
    // and divide that chunk between different allocators for entities, components and systems.
    // this whay we can make sure that everything is aligned in memory
    class IAllocator
    {
    public:
        virtual void* allocate(size_t size, size_t align) = 0;
        virtual void deallocate(void* p) = 0;
        virtual size_t allocated_size(void* p) = 0;
    };
}
