//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_DOCUMENT_HPP
#define JSON_JS_DOCUMENT_HPP


#include "types/js_entry.hpp"
#include "memory_pool.hpp"

namespace json
{
	class document
	{
	public:
		document();

		explicit document(size_t pool_size);

		friend class object;

		friend class array;

		friend class parser;

	protected:
		[[nodiscard]] MemoryPool* get_pool();

		MemoryPool pool;
	};
}


#endif //JSON_JS_DOCUMENT_HPP
