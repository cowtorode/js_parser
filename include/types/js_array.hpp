//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_ARRAY_HPP
#define JSON_JS_ARRAY_HPP


#include <string>
#include <vector>
#include "js_data.hpp"
#include "js_result.hpp"

class js_object;

/**
 * Attempts to access a specific typed value from an array element.
 *
 * All accessor functions prefixed with get_ will:
 *  - Return js_result::OK and write to the output if the value exists and matches the expected type.
 *  - Return js_result::NULL_DATA if the value is explicitly null.
 *  - Return js_result::INVALID_INDEX if the index is out of bounds.
 *  - Return js_result::TYPE_MISMATCH if the value exists but is not of the requested type.
 *
 * On failure (anything but js_result::OK), the output parameter is left unmodified.
 */
class js_array : public js_data
{
public:
    js_array();

    ~js_array();

    void add(js_data* data);

    js_result get_object(int index, js_object*& out) const;

    js_result get_bool(int index, bool& out) const;

    js_result get_number(int index, double& out) const;

    js_result get_string(int index, std::string& out) const;

    js_result get_array(int index, js_array*& out) const;
protected:
    std::vector<js_data*> array;
};


#endif //JSON_JS_ARRAY_HPP
