//
// Created by cory on 6/24/25.
//

#include <cstdint>
#include <cstdlib>
#include "memory_pool.hpp"

MemoryPool::MemoryPool(size_t pool_size) : pool(malloc(pool_size)), cursor((char*) pool), remaining(0)
{
    if (pool)
    {
        remaining = pool_size;
    }
}

MemoryPool::~MemoryPool()
{
    free(pool);
}

void* MemoryPool::palloc(size_t size, size_t alignment)
{
    std::uintptr_t raw = reinterpret_cast<std::uintptr_t>(cursor);
    std::uintptr_t aligned = (raw + alignment - 1) & ~(alignment - 1);
    // how many bytes need added to properly align this type
    std::size_t padding = aligned - raw;

    if (size + padding > remaining)
    {
        return nullptr;
    }

    remaining -= size + padding;
    cursor += size + padding;

    return reinterpret_cast<void*>(aligned);
}

ptrdiff_t MemoryPool::allocated() const
{
    return cursor - (char*) pool;
}
