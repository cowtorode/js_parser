//
// Created by cory on 6/24/25.
//

#include <cstdint>
#include <cstdlib>
#include "json/types/document/memory_pool.hpp"

MemoryPool::MemoryPool() : sizeof_pool(0), pool(nullptr), cursor(nullptr), remaining(0) //: MemoryPool(1024)
{}

MemoryPool::MemoryPool(size_t pool_size) : sizeof_pool(1), pool((void**) malloc(pool_size)), cursor((char*) pool), remaining(0)
{
    if (pool)
    {
        remaining = pool_size;
    }
}

MemoryPool::~MemoryPool()
{
	for (size_t i = 0; i < sizeof_pool; ++i)
	{
		free(pool[i]);
	}

    free(pool);
}

void MemoryPool::resize()
{
	// create new page list
	size_t new_sizeof_pool = sizeof_pool + 1;
	void** new_pool = reinterpret_cast<void**>(malloc(sizeof(void*) * new_sizeof_pool));

	// assign already existing pages to new page list
	for (size_t i = 0; i < sizeof_pool; ++i)
	{
		new_pool[i] = pool[i];
	}

	// allocate new page and append it to page list
	cursor = (char*) malloc(1024);
	remaining = 1024;
	new_pool[sizeof_pool] = cursor;

	// free old page list
	free(pool);

	// assign new page list
	sizeof_pool = new_sizeof_pool;
	pool = new_pool;
}

void* MemoryPool::palloc(size_t size, size_t alignment)
{
	if (!cursor)
	{
		// todo explicitly pass worst case scenario for bytes required to ensure this fits on next call
		resize();
	}

    uintptr_t raw = reinterpret_cast<uintptr_t>(cursor);
    uintptr_t aligned = (raw + alignment - 1) & ~(alignment - 1);
    // how many bytes need added to properly align this type
    size_t aligned_size = aligned - raw + size;

    if (aligned_size > remaining)
    {
		// todo explicitly pass aligned_size to ensure this fits on next call
		resize();

		if (aligned_size > remaining)
		{
			return nullptr;
		}
    }

    remaining -= aligned_size;
    cursor += aligned_size;

    return reinterpret_cast<void*>(aligned);
}

ptrdiff_t MemoryPool::allocated() const
{
    return cursor - (char*) pool;
}
