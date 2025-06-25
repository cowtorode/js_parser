//
// Created by cory on 6/24/25.
//

#include <sstream>
#include "js_root_array.hpp"

namespace json
{
	root_array::root_array(size_t size) : js_root(size), array()
	{}

	std::string root_array::json_string() const
	{
		std::stringstream ss;

		build_json_string(ss);

		return ss.str();
	}
}