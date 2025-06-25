//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_BOOL_HPP
#define JSON_JS_BOOL_HPP


#include "js_data.hpp"

class js_bool : public js_data
{
public:
    explicit js_bool(bool boolean);

    [[nodiscard]] bool bool_value() const;

private:
    bool boolean;
};


#endif //JSON_JS_BOOL_HPP
