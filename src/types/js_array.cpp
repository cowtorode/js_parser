//
// Created by cory on 6/14/25.
//

#include "js_destructor.hpp"
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_bool.hpp"
#include "types/js_number.hpp"
#include "types/js_string.hpp"

js_array::js_array() : js_data(ARRAY) {}

js_array::~js_array()
{
    for (int i = 0; i < array.size(); ++i)
    {
        destroy_js_data(array[i]);
    }
}

void js_array::add(js_data* data)
{
    array.push_back(data);
}

js_result js_array::get_object(int i, js_object*& object) const
{
    if (i >= array.size())
    {
        return INVALID_INDEX; // array out of bounds
    }

    // get the data at the index
    js_data* data = array[i];

    if (!data)
    {
        return NULL_DATA; // data is null
    }

    if (data->type == OBJECT)
    {
        // data is a serialized as a js_object
        object = ((js_object*) data);
        return OK;
    }

    return TYPE_MISMATCH; // data is not a js_object
}

js_result js_array::get_bool(int i, bool& b) const
{
    if (i >= array.size())
    {
        return INVALID_INDEX; // array out of bounds
    }

    // get the data at the index
    js_data* data = array[i];

    if (!data)
    {
        return NULL_DATA; // data is null
    }

    if (data->type == BOOL)
    {
        // data is a serialized as a bool
        b = ((js_bool*) data)->bool_value();
        return OK;
    }

    return TYPE_MISMATCH; // data is not a bool
}

js_result js_array::get_number(int i, double &d) const
{
    if (i >= array.size())
    {
        return INVALID_INDEX; // array out of bounds
    }

    // get the data at the index
    js_data* data = array[i];

    if (!data)
    {
        return NULL_DATA; // data is null
    }

    if (data->type == NUMBER)
    {
        // data is a serialized as a number
        d = ((js_number*) data)->double_value();
        return OK;
    }

    return TYPE_MISMATCH; // data is not a number
}

js_result js_array::get_string(int i, std::string& str) const
{
    if (i >= array.size())
    {
        return INVALID_INDEX; // array out of bounds
    }

    // get the data at the index
    js_data* data = array[i];

    if (!data)
    {
        return NULL_DATA; // data is null
    }

    if (data->type == STRING)
    {
        // data is a serialized as a string
        str = ((js_string*) data)->string_value();
        return OK;
    }

    return TYPE_MISMATCH; // data is not a string
}

js_result js_array::get_array(int i, js_array*& out) const
{
    if (i >= array.size())
    {
        return INVALID_INDEX; // array out of bounds
    }

    // get the data at the index
    js_data* data = array[i];

    if (!data)
    {
        return NULL_DATA; // data is null
    }

    if (data->type == ARRAY)
    {
        // data is a serialized as an array
        out = ((js_array*) data);
        return OK;
    }

    return TYPE_MISMATCH; // data is not an array
}
