//
// Created by cory on 6/12/25.
//

#include <iostream>
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_string.hpp"
#include "js_root_array.hpp"
#include "parse/js_parser.hpp"
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

static constexpr const char* example_json = R"([{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"}])";

void parse_json_string()
{
	json::parser parser;

	json::entry* array = parser.parse(example_json);

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

void parse_json_file()
{
	json::parser parser;

	json::file example("/home/cory/CLionProjects/json/example/example.json");

	example.open_file();

	json::entry* array = parser.parse(example);

	example.close_file();

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

void parse_json_string_and_serialize_to_json_string()
{
	json::parser parser;

	json::entry* array = parser.parse(R"([{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"},{},[]])");

	if (array->type == json::ARRAY)
	{
		std::cout << ((json::root_array*) array)->json_string() << std::endl;
	}

	delete (json::root_array*) array;
}

int main()
{
    // json string
    // multiple file parsing with the same parser
    // parse all data types
    // after how many objects nested does the parser fail?
    // modifying a root object after its construction and rewriting (json string)
    // benchmarking

	//parse_json_string();
	//parse_json_file();
	parse_json_string_and_serialize_to_json_string();

    return 0;
}