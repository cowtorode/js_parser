//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_NUMBER_HPP
#define JSON_JS_NUMBER_HPP


#include <sstream>
#include "js_entry.hpp"

namespace json
{
	class number : public entry
	{
	public:
		explicit number(double x);

		[[nodiscard]] double double_value() const;

		void build_json_string(std::stringstream& ss) const;

	private:
		double x;
	};
}


#endif //JSON_JS_NUMBER_HPP