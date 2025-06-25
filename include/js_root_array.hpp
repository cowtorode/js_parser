//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ROOT_ARRAY_HPP
#define JSON_JS_ROOT_ARRAY_HPP


#include "js_root.hpp"
#include "types/js_array.hpp"

namespace json
{
	class root_array : public js_root, public array
	{
	public:
		explicit root_array(size_t size);

		[[nodiscard]] std::string json_string() const;
	};
}


#endif //JSON_JS_ROOT_ARRAY_HPP