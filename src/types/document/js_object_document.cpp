//
// Created by cory on 6/24/25.
//

#include <sstream>
#include "types/document/js_object_document.hpp"

namespace json
{
	object_document::object_document(size_t size) : document(size), object() {}

	std::string object_document::json_string() const
	{
		std::stringstream ss;

		build_json_string(ss);

		return ss.str();
	}
}