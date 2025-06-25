//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ROOT_OBJECT_HPP
#define JSON_JS_ROOT_OBJECT_HPP


#include "js_root.hpp"
#include "types/js_object.hpp"
#include "memory_pool.hpp"

namespace json
{
	class root_object : public js_root, public object
	{
	public:
		explicit root_object(size_t size);
	};
}


#endif //JSON_JS_ROOT_OBJECT_HPP