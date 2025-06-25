//
// Created by cory on 6/24/25.
//

#ifndef JSON_MEMORY_POOL_HPP
#define JSON_MEMORY_POOL_HPP


#include <cstddef>

class MemoryPool
{
public:
    ~MemoryPool();

    [[nodiscard]] ptrdiff_t allocated() const;

    explicit MemoryPool(size_t pool_size);

    /**
     * Pool allocation
     */
    void* palloc(size_t size, size_t alignment);

private:
    void* pool; // do not touch this
    /**
     * The next address that's returned from allocate, assuming there's enough bytes remaining.
     */
    char* cursor;
    size_t remaining;
};


#endif //JSON_MEMORY_POOL_HPP
