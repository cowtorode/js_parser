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
}