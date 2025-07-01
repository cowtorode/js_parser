//
// Created by cory on 6/24/25.
//

#include "json/util/js_destructor.hpp"
#include "json/types/js_object.hpp"
#include "json/types/js_bool.hpp"
#include "json/types/js_number.hpp"
#include "json/types/js_string.hpp"
#include "json/types/js_array.hpp"

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