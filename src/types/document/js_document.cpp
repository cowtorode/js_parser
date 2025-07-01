//
// Created by cory on 6/24/25.
//

#include "json/types/document/js_document.hpp"

namespace json
{
	document::document() : pool()
	{}

	document::document(size_t pool_size) : pool(pool_size)
	{}

	MemoryPool* document::get_pool()
	{
		return &pool;
	}
}
