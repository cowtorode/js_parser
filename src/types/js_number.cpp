//
// Created by cory on 6/14/25.
//

#include "types/js_number.hpp"

namespace json
{
	number::number(double x) : entry(NUMBER), x(x)
	{}

	double number::double_value() const
	{
		return x;
	}
}