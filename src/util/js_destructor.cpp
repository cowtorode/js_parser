//
// Created by cory on 6/24/25.
//

#include "util/js_destructor.hpp"
#include "types/js_object.hpp"
#include "types/js_bool.hpp"
#include "types/js_number.hpp"
#include "types/js_string.hpp"
#include "types/js_array.hpp"

namespace json
{
	void destroy_entry(entry* data)
	{
		switch (data->type)
		{
			case OBJECT:
				((object*) data)->~object();
				break;
			case BOOL:
				((boolean*) data)->~boolean();
				break;
			case NUMBER:
				((number*) data)->~number();
				break;
			case STRING:
				((string*) data)->~string();
				break;
			case ARRAY:
				((array*) data)->~array();
				break;
		}
	}
}