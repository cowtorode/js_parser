//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_STRING_HPP
#define JSON_JS_STRING_HPP


#include <string>
#include "js_data.hpp"

class js_string : public js_data
{
public:
    explicit js_string(std::string str);

    [[nodiscard]] const std::string& string_value() const;
private:
    std::string str;
};


#endif //JSON_JS_STRING_HPP
