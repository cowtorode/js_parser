//
// Created by cory on 6/24/25.
//

#include <sstream>
#include "types/document/js_array_document.hpp"

namespace json
{
	array_document::array_document() : document(), array()
	{}

	array_document::array_document(size_t size) : document(size), array()
	{}

	std::string array_document::json_string() const
	{
		std::stringstream ss;

		build_json_string(ss);

		return ss.str();
	}
}