//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ROOT_HPP
#define JSON_JS_ROOT_HPP


#include "types/js_data.hpp"
#include "memory_pool.hpp"

class js_root
{
public:
    explicit js_root(size_t pool_size);

    friend class js_object;

    friend class js_array;

    friend class JsonParser;

protected:
    [[nodiscard]] MemoryPool* get_pool();

    MemoryPool pool;
};


#endif //JSON_JS_ROOT_HPP
