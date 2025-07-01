//
// Created by cory on 6/24/25.
//

#ifndef JSON_MEMORY_POOL_HPP
#define JSON_MEMORY_POOL_HPP


#include <cstddef>

class MemoryPool
{
public:
	MemoryPool();

	explicit MemoryPool(size_t pool_size);

    ~MemoryPool();

    [[nodiscard]] ptrdiff_t allocated() const;

	void resize();

    /**
     * Pool allocation
     */
    void* palloc(size_t size, size_t alignment);

private:
	size_t sizeof_pool;
	/**
	 * The list of memory pages allocated to this MemoryPool. On destruction, this list needs
	 * deleted, as well as all of its elements.
	 */
    void** pool;
    /**
     * The next address that's returned from allocate, assuming there's enough bytes remaining.
     */
    char* cursor;
    size_t remaining;
};


#endif //JSON_MEMORY_POOL_HPP
