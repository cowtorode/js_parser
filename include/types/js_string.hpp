//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_STRING_HPP
#define JSON_JS_STRING_HPP


#include <sstream>
#include "js_entry.hpp"

namespace json
{
	class string : public entry
	{
	public:
		explicit string(std::string str);

		[[nodiscard]] const std::string& string_value() const;

		void build_json_string(std::stringstream& ss) const;

	private:
		std::string str;
	};
}


#endif //JSON_JS_STRING_HPP