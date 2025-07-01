//
// Created by cory on 6/12/25.
//

#include <iostream>
#include "types/js_object.hpp"
#include "types/js_array.hpp"
#include "types/js_string.hpp"
#include "types/document/js_array_document.hpp"
#include "parse/js_parser.hpp"
#include "io/file.hpp"
#include "parse/route/router.hpp"

struct element
{
	double z;
	std::string symbol;
	std::string name;
};

struct table_of_elements
{
	element elements[2];
};

void print_element(const element& ele)
{
	std::cout << ele.z << ". " << ele.name << " (" << ele.symbol << ')' << std::endl;
}

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

static constexpr const char* elements_json = R"([{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"}])";

void parse_json_string_root_array()
{
	json::parser parser;
	json::array_document array;

	if (parser.parse_into(array, elements_json))
	{
		// success!
		json::object* jso;

		for (int i = 0; i < 2; ++i)
		{
			if (array.get_object(i, jso) == OK)
			{
				print_element(jso);
			}
		}
	} else if (parser.code())
	{
		// error occurred while parsing
		parser.print_code();
	} else
	{
		// the file points to an object_document
	}
}

void parse_json_file_root_array()
{
	json::parser parser;
	json::array_document array;
	json::file file("/home/cory/CLionProjects/json/example/example.json");

	file.open_file();

	if (parser.parse_into(array, file))
	{
		// success!
		json::object* jso;

		for (int i = 0; i < 2; ++i)
		{
			if (array.get_object(i, jso) == OK)
			{
				print_element(jso);
			}
		}
	} else if (parser.code())
	{
		// error occurred while parsing
		parser.print_code();
	} else
	{
		// the file points to an object_document
	}

	// this is implicit, but I've included it here to showcase all control flow
	file.close_file();
}

void parse_json_string_root_array_and_serialize_to_json_string_root_array()
{
	json::parser parser;
	json::array_document array;

	if (parser.parse_into(array, R"([{"z":1,"symbol":"H","name":"Hydrogen"},{"z":2,"symbol":"He","name":"Helium"},{},[]])"))
	{
		// success!
		std::cout << array.json_string() << std::endl;
	}
}

void parse_json_string_root_array_user_defined()
{
	json::router root;

	json::router element_router;

//	element_router.set_

//	root.set_array_router(element_router);

//	json::parser parser;

//	parser.set_router(root);

	table_of_elements table;

//	parser.parse(elements_json, table);

	for (int i = 0; i < 2; ++i)
	{
		print_element(table.elements[i]);
	}
}

int main()
{
    // json string
	// array_document and object_document creation w/o parser
    // multiple file parsing with the same parser
    // parse all data types
    // after how many objects nested does the parser fail?
    // modifying a root object after its construction and rewriting (json string)
    // benchmarking

	parse_json_string_root_array();
	//parse_json_file_root_array();
	//when your function name is 68 chars long:
	//parse_json_string_root_array_and_serialize_to_json_string_root_array();
	//parse_json_string_root_array_user_defined();

    return 0;
}