//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ROOT_ARRAY_HPP
#define JSON_JS_ROOT_ARRAY_HPP


#include "js_root.hpp"
#include "types/js_array.hpp"

class js_root_array : public js_root, public js_array
{
public:
    explicit js_root_array(size_t size);

    std::string json_string();

    [[deprecated]] js_object* add_object();
};


#endif //JSON_JS_ROOT_ARRAY_HPP
