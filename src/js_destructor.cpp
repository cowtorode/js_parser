//
// Created by cory on 6/24/25.
//

#include "js_destructor.hpp"
#include "types/js_object.hpp"
#include "types/js_bool.hpp"
#include "types/js_number.hpp"
#include "types/js_string.hpp"
#include "types/js_array.hpp"

void destroy_js_data(js_data* data)
{
    switch (data->type)
    {
        case OBJECT:
            ((js_object*) data)->~js_object();
            break;
        case BOOL:
            ((js_bool*) data)->~js_bool();
            break;
        case NUMBER:
            ((js_number*) data)->~js_number();
            break;
        case STRING:
            ((js_string*) data)->~js_string();
            break;
        case ARRAY:
            ((js_array*) data)->~js_array();
            break;
    }
}