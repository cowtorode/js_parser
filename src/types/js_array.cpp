//
// Created by cory on 6/14/25.
//

#include "js_destructor.hpp"
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
