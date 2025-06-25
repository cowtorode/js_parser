//
// Created by cory on 6/24/25.
//

#include <sstream>
#include "js_root_object.hpp"

namespace json
{
	root_object::root_object(size_t size) : js_root(size), object() {}

	std::string root_object::json_string() const
	{
		std::stringstream ss;

		build_json_string(ss);

		return ss.str();
	}
}