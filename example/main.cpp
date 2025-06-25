//
// Created by cory on 6/12/25.
//

#include <iostream>
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_string.hpp"
#include "js_root_array.hpp"
#include "js_parser.hpp"
#include "io/file.hpp"

void print_element(json::object* obj)
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
	json::parser parser;

	json::entry* array = parser.parse(R"([{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"}])");

	if (array->type == json::ARRAY)
	{
		json::object* jso;

		for (int i = 0; i < 2; ++i)
		{
			if (((json::array*) array)->get_object(i, jso) == OK)
			{
				print_element(jso);
			}
		}
	}

	delete (json::root_array*) array;
}

void test2()
{
	json::parser parser;

	json::file example("/home/cory/CLionProjects/json/example/example.json");

	parser.parse(example);
}

int main()
{
    // json string
    // multiple file parsing with the same parser
    // parse all data types
    // after how many objects nested does the parser fail?
    // modifying a root object after its construction and rewriting (json string)
    // benchmarking

    test1();
    return 0;
}