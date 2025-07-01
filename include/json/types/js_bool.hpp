//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_BOOL_HPP
#define JSON_JS_BOOL_HPP


#include <sstream>
#include "js_entry.hpp"

namespace json
{
	class boolean : public entry
	{
	public:
		explicit boolean(bool x);

		[[nodiscard]] bool bool_value() const;

		void build_json_string(std::stringstream& ss) const;

	private:
		bool x;
	};
}


#endif //JSON_JS_BOOL_HPP
