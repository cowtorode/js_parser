//
// Created by cory on 6/14/25.
//

#include <utility>
#include "types/js_string.hpp"

js_string::js_string(std::string str) : js_data(STRING), str(std::move(str)) {}

const std::string& js_string::string_value() const
{
    return str;
}
