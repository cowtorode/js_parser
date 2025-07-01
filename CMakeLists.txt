cmake_minimum_required(VERSION 3.31)
project(json)

set(CMAKE_CXX_STANDARD 20)

add_library(json STATIC
        src/parse/js_parser.cpp
        include/parse/js_parser.hpp
        include/types/js_type.hpp
        src/types/js_entry.cpp
        include/types/js_entry.hpp
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
        include/types/document/js_document.hpp
        src/types/document/js_object_document.cpp
        include/types/document/js_object_document.hpp
        src/types/document/js_array_document.cpp
        include/types/document/js_array_document.hpp
        include/util/js_destructor.hpp
        src/util/js_destructor.cpp
        src/types/document/memory_pool.cpp
        include/types/document/memory_pool.hpp
        src/types/document/js_document.cpp
        src/io/file.cpp
        include/io/file.hpp
        src/parse/route/router.cpp
        include/parse/route/router.hpp
        include/util/math.hpp
        src/util/math.cpp
)

# This makes the include/ directory available to anything that links json
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(example)
