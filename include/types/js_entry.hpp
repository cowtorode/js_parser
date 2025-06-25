//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_ENTRY_HPP
#define JSON_JS_ENTRY_HPP


#include "js_type.hpp"

namespace json
{
	class entry
	{
	public:
		const enum type type;

	protected:
		explicit entry(enum type type);
	};
}


#endif //JSON_JS_ENTRY_HPP