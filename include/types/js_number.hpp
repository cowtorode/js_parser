//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_NUMBER_HPP
#define JSON_JS_NUMBER_HPP


#include "js_data.hpp"

class js_number : public js_data
{
public:
    explicit js_number(double number);

    [[nodiscard]] double double_value() const;

private:
    double number;
};


#endif //JSON_JS_NUMBER_HPP
