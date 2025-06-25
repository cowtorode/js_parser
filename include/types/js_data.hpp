//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_DATA_HPP
#define JSON_JS_DATA_HPP


#include "js_type.hpp"

class js_data
{
public:
    const js_type type;

protected:
    explicit js_data(js_type type);
};


#endif //JSON_JS_DATA_HPP
