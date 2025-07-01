//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_RESULT_HPP
#define JSON_JS_RESULT_HPP

/**
 * Represents the result of attempting to extract a value from either a JSON array or JSON object.
 */
enum js_result
{
    // The value exists and is of the expected type; output parameter has been written to.
    OK,
    // The value exists but is explicitly null.
    NULL_DATA,
    // For arrays: index out of bounds. For objects: key does not exist.
    INVALID_INDEX,
    // The value exists but is not of the expected type.
    TYPE_MISMATCH
};

#endif //JSON_JS_RESULT_HPP
