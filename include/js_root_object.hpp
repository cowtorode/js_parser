//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ROOT_OBJECT_HPP
#define JSON_JS_ROOT_OBJECT_HPP


#include "js_root.hpp"
#include "types/js_object.hpp"
#include "memory_pool.hpp"

class js_root_object : public js_root, public js_object
{
public:
    explicit js_root_object(size_t size);
};


#endif //JSON_JS_ROOT_OBJECT_HPP
