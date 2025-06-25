//
// Created by cory on 6/14/25.
//

#include <iostream>
#include <sstream>
#include "js_destructor.hpp"
#include "types/js_object.hpp"
#include "types/js_bool.hpp"
#include "types/js_number.hpp"
#include "types/js_string.hpp"
#include "types/js_array.hpp"
#include "js_root.hpp"

js_object::js_object() : js_data(OBJECT) {}

js_object::~js_object()
{
    for (auto& pair : map)
    {
        destroy_js_data(pair.second);
    }
}

void js_object::json_string(std::stringstream& ss)
{
    for (auto& pair : map)
    {
        ss << '"' << pair.first << "\":";

        switch (pair.second->type)
        {
            case OBJECT:
                ss << '{';
                ((js_object*) pair.second)->json_string(ss);
                ss << '}';
                break;
            case BOOL:
                ss << (((js_bool*) pair.second)->bool_value() ? "true" : "false");
                break;
            case NUMBER:
                ss << ((js_number*) pair.second)->double_value();
                break;
            case STRING:
                ss << '"' << ((js_string*) pair.second)->string_value() << '"';
                break;
            case ARRAY:
                ss << '[';
                //...
                ss << ']';
                break;
        }
    }
}

void js_object::add(const std::string& tag, js_data* data)
{
    map[tag] = data;
}

js_result js_object::get_object(const std::string& tag, js_object*& object) const
{
    auto itr = map.find(tag);

    if (itr == map.end())
    {
        return INVALID_INDEX;
    }

    js_data* data = itr->second;

    if (!data)
    {
        return NULL_DATA;
    }

    if (data->type == OBJECT)
    {
        object = ((js_object*) data);
        return OK;
    }

    return TYPE_MISMATCH;
}

js_result js_object::get_bool(const std::string& tag, bool& b) const
{
    auto itr = map.find(tag);

    if (itr == map.end())
    {
        return INVALID_INDEX;
    }

    js_data* data = itr->second;

    if (!data)
    {
        return NULL_DATA;
    }

    if (data->type == BOOL)
    {
        b = ((js_bool*) data)->bool_value();
        return OK;
    }

    return TYPE_MISMATCH;
}

js_result js_object::get_number(const std::string& tag, double& d) const
{
    auto itr = map.find(tag);

    if (itr == map.end())
    {
        return INVALID_INDEX;
    }

    js_data* data = itr->second;

    if (!data)
    {
        return NULL_DATA;
    }

    if (data->type == NUMBER)
    {
        d = ((js_number*) data)->double_value();
        return OK;
    }

    return TYPE_MISMATCH;
}

js_result js_object::get_string(const std::string& tag, std::string& str) const
{
    auto itr = map.find(tag);

    if (itr == map.end())
    {
        return INVALID_INDEX;
    }

    js_data* data = itr->second;

    if (!data)
    {
        return NULL_DATA;
    }

    if (data->type == STRING)
    {
        str = ((js_string*) data)->string_value();
        return OK;
    }

    return TYPE_MISMATCH;
}

js_result js_object::get_array(const std::string& tag, js_array*& array) const
{
    auto itr = map.find(tag);

    if (itr == map.end())
    {
        return INVALID_INDEX;
    }

    js_data* data = itr->second;

    if (!data)
    {
        return NULL_DATA;
    }

    if (data->type == ARRAY)
    {
        array = ((js_array*) data);
        return OK;
    }

    return TYPE_MISMATCH;
}

js_object* js_object::add_object(js_root* root, const std::string& tag)
{
    js_object* jso = new (root->get_pool()->palloc(sizeof(js_object), alignof(js_object))) js_object;

    add(tag, jso);

    return jso;
}

void js_object::add_bool(js_root* root, const std::string& tag, bool x)
{
    add(tag, new (root->get_pool()->palloc(sizeof(js_bool), alignof(js_bool))) js_bool(x));
}

void js_object::add_number(js_root* root, const std::string& tag, double x)
{
    add(tag, new (root->get_pool()->palloc(sizeof(js_number), alignof(js_number))) js_number(x));
}

void js_object::add_string(js_root* root, const std::string& tag, const std::string& x)
{
    add(tag, new (root->get_pool()->palloc(sizeof(js_string), alignof(js_string))) js_string(x));
}

js_array* js_object::add_array(js_root* root, const std::string& tag)
{
    js_array* jsa = new (root->get_pool()->palloc(sizeof(js_array), alignof(js_array))) js_array;

    add(tag, jsa);

    return jsa;
}
