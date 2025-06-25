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

namespace json
{
    object::object() : entry(OBJECT)
    {}

    object::~object()
    {
        for (auto& pair: map)
        {
			destroy_entry(pair.second);
        }
    }

    void object::json_string(std::stringstream& ss)
    {
        for (auto& pair: map)
        {
            ss << '"' << pair.first << "\":";

            switch (pair.second->type)
            {
                case OBJECT:
                    ss << '{';
                    ((object*) pair.second)->json_string(ss);
                    ss << '}';
                    break;
                case BOOL:
                    ss << (((boolean*) pair.second)->bool_value() ? "true" : "false");
                    break;
                case NUMBER:
                    ss << ((number*) pair.second)->double_value();
                    break;
                case STRING:
                    ss << '"' << ((string*) pair.second)->string_value() << '"';
                    break;
                case ARRAY:
                    ss << '[';
                    //...
                    ss << ']';
                    break;
            }
        }
    }

    void object::add(const std::string& tag, entry* data)
    {
        map[tag] = data;
    }

    js_result object::get_object(const std::string& tag, object*& out) const
    {
        auto itr = map.find(tag);

        if (itr == map.end())
        {
            return INVALID_INDEX;
        }

        entry* data = itr->second;

        if (!data)
        {
            return NULL_DATA;
        }

        if (data->type == OBJECT)
        {
			out = ((object*) data);
            return OK;
        }

        return TYPE_MISMATCH;
    }

    js_result object::get_bool(const std::string& tag, bool& out) const
    {
        auto itr = map.find(tag);

        if (itr == map.end())
        {
            return INVALID_INDEX;
        }

        entry* data = itr->second;

        if (!data)
        {
            return NULL_DATA;
        }

        if (data->type == BOOL)
        {
			out = ((boolean*) data)->bool_value();
            return OK;
        }

        return TYPE_MISMATCH;
    }

    js_result object::get_number(const std::string& tag, double& out) const
    {
        auto itr = map.find(tag);

        if (itr == map.end())
        {
            return INVALID_INDEX;
        }

        entry* data = itr->second;

        if (!data)
        {
            return NULL_DATA;
        }

        if (data->type == NUMBER)
        {
			out = ((number*) data)->double_value();
            return OK;
        }

        return TYPE_MISMATCH;
    }

    js_result object::get_string(const std::string& tag, std::string& out) const
    {
        auto itr = map.find(tag);

        if (itr == map.end())
        {
            return INVALID_INDEX;
        }

        entry* data = itr->second;

        if (!data)
        {
            return NULL_DATA;
        }

        if (data->type == STRING)
        {
			out = ((string*) data)->string_value();
            return OK;
        }

        return TYPE_MISMATCH;
    }

    js_result object::get_array(const std::string& tag, array*& out) const
    {
        auto itr = map.find(tag);

        if (itr == map.end())
        {
            return INVALID_INDEX;
        }

        entry* data = itr->second;

        if (!data)
        {
            return NULL_DATA;
        }

        if (data->type == ARRAY)
        {
			out = ((array*) data);
            return OK;
        }

        return TYPE_MISMATCH;
    }

    object* object::add_object(js_root* root, const std::string& tag)
    {
        object* jso = new(root->get_pool()->palloc(sizeof(object), alignof(object))) object;

        add(tag, jso);

        return jso;
    }

    void object::add_bool(js_root* root, const std::string& tag, bool x)
    {
        add(tag, new(root->get_pool()->palloc(sizeof(boolean), alignof(boolean))) boolean(x));
    }

    void object::add_number(js_root* root, const std::string& tag, double x)
    {
        add(tag, new(root->get_pool()->palloc(sizeof(number), alignof(number))) number(x));
    }

    void object::add_string(js_root* root, const std::string& tag, const std::string& x)
    {
        add(tag, new(root->get_pool()->palloc(sizeof(string), alignof(string))) string(x));
    }

    array* object::add_array(js_root* root, const std::string& tag)
    {
        array* jsa = new(root->get_pool()->palloc(sizeof(array), alignof(array))) array;

        add(tag, jsa);

        return jsa;
    }
}
