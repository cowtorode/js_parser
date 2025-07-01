//
// Created by cory on 6/14/25.
//

#include <utility>
#include "json/types/js_string.hpp"

namespace json
{
	string::string(std::string str) : entry(STRING), str(std::move(str))
	{}

	const std::string& string::string_value() const
	{
		return str;
	}

	void string::build_json_string(std::stringstream& ss) const
	{
		ss << '"' << str << '"';
	}
}