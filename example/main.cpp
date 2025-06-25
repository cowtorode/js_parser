//
// Created by cory on 6/12/25.
//

#include <iostream>
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_string.hpp"
#include "js_root_array.hpp"
#include "js_parser.hpp"

void print_element(js_object* obj)
{
    // 1. Hydrogen (H)
    // 2. Helium (He)

    double num;
    std::string name;
    std::string symbol;

    if (obj->get_number("z", num) == OK)
    {
        std::cout << num << ". " << std::flush;
    }

    if (obj->get_string("name", name) == OK)
    {
        std::cout << name << " (" << std::flush;
    }

    if (obj->get_string("symbol", symbol) == OK)
    {
        std::cout << symbol << ')' << std::flush;
    }

    std::cout << std::endl;
}

void test1()
{
    // [{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"}]

    js_root_array array(4096);

    js_object* hydrogen = array.add_object();

    hydrogen->add_number(&array, "z", 1);
    hydrogen->add_string(&array, "symbol", "H");
    hydrogen->add_string(&array, "name", "Hydrogen");

    js_object* helium = array.add_object();

    helium->add_number(&array, "z", 2);
    helium->add_string(&array, "symbol", "He");
    helium->add_string(&array, "name", "Helium");

    // js object is constructed, here's how you access it:

    std::cout << array.json_string() << std::endl;

    js_object* jso;

    for (int i = 0; i < 2; ++i)
    {
        if (array.get_object(i, jso) == OK)
        {
            print_element(jso);
        }
    }
}

int main()
{
    //test1();

    JsonParser parser{"/home/cory/CLionProjects/json/example/example.json"};

    parser.open_file();

    js_data* array = parser.parse();

    parser.close_file();

    if (array->type == ARRAY)
    {
        js_object* jso;

        for (int i = 0; i < 2; ++i)
        {
            if (((js_array*) array)->get_object(i, jso) == OK)
            {
                print_element(jso);
            }
        }
    }

    delete (js_root_array*) array;

    return 0;
}