//
// Created by cory on 6/24/25.
//

#include <iostream>
#include <sstream>
#include "js_root_array.hpp"
#include "types/js_object.hpp"
#include "types/js_bool.hpp"
#include "types/js_string.hpp"

namespace json
{
	root_array::root_array(size_t size) : js_root(size), array()
	{}

	object* root_array::add_object()
	{
		object* jso = new(pool.palloc(sizeof(object), alignof(object))) object;

		add(jso);

		std::cout << "  added object" << std::endl;

		return jso;
	}

	std::string root_array::json_string()
	{
		std::stringstream ss;

		ss << '[';

		for (entry* data: vec)
		{
			switch (data->type)
			{
				case OBJECT:
					ss << '{';
					((object*) data)->json_string(ss);
					ss << '}';
					break;
				case BOOL:
					break;
				case NUMBER:
					break;
				case STRING:
					break;
				case ARRAY:
					break;
			}
		}

		ss << ']';
		return ss.str();
	}
}