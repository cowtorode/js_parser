//
// Created by cory on 6/14/25.
//

#include "types/js_bool.hpp"

js_bool::js_bool(bool boolean) : js_data(BOOL), boolean(boolean) {}

bool js_bool::bool_value() const
{
    return boolean;
}