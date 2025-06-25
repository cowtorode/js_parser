//
// Created by cory on 6/24/25.
//

#include <iostream>
#include <sstream>
#include "js_root_array.hpp"
#include "types/js_object.hpp"
#include "types/js_bool.hpp"
#include "types/js_string.hpp"

js_root_array::js_root_array(size_t size) : js_root(size), js_array()
{}

js_object* js_root_array::add_object()
{
    js_object* jso = new (pool.palloc(sizeof(js_object), alignof(js_object))) js_object;

    add(jso);

    std::cout << "  added js_object" << std::endl;

    return jso;
}

std::string js_root_array::json_string()
{
    std::stringstream ss;

    ss << '[';

    for (js_data* data : array)
    {
        switch (data->type)
        {
            case OBJECT:
                ss << '{';
                ((js_object*) data)->json_string(ss);
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
