//
// Created by cory on 6/14/25.
//

#include "util/js_destructor.hpp"
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_bool.hpp"
#include "types/js_number.hpp"
#include "types/js_string.hpp"

namespace json
{
	array::array() : entry(ARRAY)
	{}

	array::~array()
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			destroy_entry(vec[i]);
		}
	}

	void array::build_json_string(std::stringstream& ss) const
	{
		ss << '[';

		// this check is necessary since the jmp statement skips over the first while loop check.
		if (vec.empty())
		{
			ss << ']';
			return;
		}

		entry* data;

		unsigned long i = 0;

		goto switch_case; // skip the comma prefix for the first array element

		while (i < vec.size())
		{
			ss << ',';

			switch_case:

			data = vec[i];

			// todo: make this into util method as it's duplicated in js_object.cpp
			switch (data->type)
			{
				case OBJECT:
					((object*) data)->build_json_string(ss);
					break;
				case BOOL:
					((boolean*) data)->build_json_string(ss);
					break;
				case NUMBER:
					((number*) data)->build_json_string(ss);
					break;
				case STRING:
					((string*) data)->build_json_string(ss);
					break;
				case ARRAY:
					((array*) data)->build_json_string(ss);
					break;
			}

			++i;
		}

		ss << ']';
	}

	void array::add(entry* data)
	{
		vec.push_back(data);
	}

	js_result array::get_object(int i, object*& out) const
	{
		if (i >= vec.size())
		{
			return INVALID_INDEX; // array out of bounds
		}

		// get the data at the index
		entry* data = vec[i];

		if (!data)
		{
			return NULL_DATA; // data is null
		}

		if (data->type == OBJECT)
		{
			// data is a serialized as an object
			out = ((object*) data);
			return OK;
		}

		return TYPE_MISMATCH; // data is not an object
	}

	js_result array::get_bool(int i, bool& out) const
	{
		if (i >= vec.size())
		{
			return INVALID_INDEX; // array out of bounds
		}

		// get the data at the index
		entry* data = vec[i];

		if (!data)
		{
			return NULL_DATA; // data is null
		}

		if (data->type == BOOL)
		{
			// data is a serialized as a bool
			out = ((boolean*) data)->bool_value();
			return OK;
		}

		return TYPE_MISMATCH; // data is not a bool
	}

	js_result array::get_number(int i, double& out) const
	{
		if (i >= vec.size())
		{
			return INVALID_INDEX; // array out of bounds
		}

		// get the data at the index
		entry* data = vec[i];

		if (!data)
		{
			return NULL_DATA; // data is null
		}

		if (data->type == NUMBER)
		{
			// data is a serialized as a number
			out = ((number*) data)->double_value();
			return OK;
		}

		return TYPE_MISMATCH; // data is not a x
	}

	js_result array::get_string(int i, std::string& out) const
	{
		if (i >= vec.size())
		{
			return INVALID_INDEX; // array out of bounds
		}

		// get the data at the index
		entry* data = vec[i];

		if (!data)
		{
			return NULL_DATA; // data is null
		}

		if (data->type == STRING)
		{
			// data is a serialized as a string
			out = ((string*) data)->string_value();
			return OK;
		}

		return TYPE_MISMATCH; // data is not a string
	}

	js_result array::get_array(int i, array*& out) const
	{
		if (i >= vec.size())
		{
			return INVALID_INDEX; // array out of bounds
		}

		// get the data at the index
		entry* data = vec[i];

		if (!data)
		{
			return NULL_DATA; // data is null
		}

		if (data->type == ARRAY)
		{
			// data is a serialized as an array
			out = ((array*) data);
			return OK;
		}

		return TYPE_MISMATCH; // data is not an array
	}
}
