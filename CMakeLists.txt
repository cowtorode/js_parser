cmake_minimum_required(VERSION 3.31)
project(json)

set(CMAKE_CXX_STANDARD 20)

add_library(json STATIC
        src/js_parser.cpp
        include/js_parser.hpp
        include/types/js_type.hpp
        src/types/js_data.cpp
        include/types/js_data.hpp
        src/types/js_object.cpp
        include/types/js_object.hpp
        src/types/js_array.cpp
        include/types/js_array.hpp
        src/types/js_bool.cpp
        include/types/js_bool.hpp
        src/types/js_number.cpp
        include/types/js_number.hpp
        src/types/js_string.cpp
        include/types/js_string.hpp
        include/types/js_result.hpp
        include/js_root.hpp
        src/js_root_object.cpp
        include/js_root_object.hpp
        src/js_root_array.cpp
        include/js_root_array.hpp
        include/js_destructor.hpp
        src/js_destructor.cpp
        src/memory_pool.cpp
        include/memory_pool.hpp
        src/js_root.cpp
)

# This makes the include/ directory available to anything that links json
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(example)
