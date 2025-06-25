//
// Created by cory on 6/14/25.
//

#include "types/js_number.hpp"

js_number::js_number(double number) : js_data(NUMBER), number(number) {}

double js_number::double_value() const
{
    return number;
}
