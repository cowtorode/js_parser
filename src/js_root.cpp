//
// Created by cory on 6/24/25.
//

#include "js_root.hpp"

js_root::js_root(size_t pool_size) : pool(pool_size)
{}

MemoryPool* js_root::get_pool()
{
    return &pool;
}
