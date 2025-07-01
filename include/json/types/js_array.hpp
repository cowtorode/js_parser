//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_ARRAY_HPP
#define JSON_JS_ARRAY_HPP


#include <string>
#include <vector>
#include "js_entry.hpp"
#include "js_result.hpp"

namespace json
{
	class object;

	/**
	 * Attempts to access a specific typed value from an array element.
	 *
	 * All accessor functions prefixed with get_ will:
	 *  - Return js_result::OK and write to the output if the value exists and matches the expected type.
	 *  - Return js_result::NULL_DATA if the value is explicitly null.
	 *  - Return js_result::INVALID_INDEX if the index is out of bounds.
	 *  - Return js_result::TYPE_MISMATCH if the value exists but is not of the requested type.
	 *
	 * On failure (anything but js_result::OK), the output parameter is left unmodified.
	 */
	class array : public entry
	{
	public:
		array();

		~array();

		void build_json_string(std::stringstream& ss) const;

		void add(entry* data);

		js_result get_object(int index, object*& out) const;

		js_result get_bool(int index, bool& out) const;

		js_result get_number(int index, double& out) const;

		js_result get_string(int index, std::string& out) const;

		js_result get_array(int index, array*& out) const;

	protected:
		std::vector<entry*> vec;
	};
}


#endif //JSON_JS_ARRAY_HPP
