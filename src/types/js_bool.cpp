//
// Created by cory on 6/14/25.
//

#include "types/js_bool.hpp"

namespace json
{
	boolean::boolean(bool x) : entry(BOOL), x(x)
	{}

	bool boolean::bool_value() const
	{
		return x;
	}

	void boolean::build_json_string(std::stringstream& ss) const
	{
		if (x)
		{
			ss << "true";
		} else
		{
			ss << "false";
		}
	}
}