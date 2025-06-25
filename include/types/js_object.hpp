//
// Created by cory on 6/14/25.
//

#ifndef JSON_JS_OBJECT_HPP
#define JSON_JS_OBJECT_HPP


#include <string>
#include <unordered_map>
#include "js_entry.hpp"
#include "js_result.hpp"

namespace json
{
	class array;

	class js_root;

	/**
	 * Attempts to access a specific typed value from a named key in a JSON object.
	 *
	 * All accessor functions prefixed with get_ will:
	 *  - Return js_result::OK and write to the output argument if the key exists and value is of the expected type.
	 *  - Return js_result::NULL_DATA if the key exists but the value is null.
	 *  - Return js_result::INVALID_INDEX if the key does not exist.
	 *  - Return js_result::TYPE_MISMATCH if the key exists but the value is of the wrong type.
	 *
	 * On failure (anything but js_result::OK), the output parameter remains unmodified.
	 */
	class object : public entry
	{
	public:
		object();

		~object();

		void build_json_string(std::stringstream& ss) const;

		js_result get_object(const std::string& tag, object*& out) const;

		js_result get_bool(const std::string& tag, bool& out) const;

		js_result get_number(const std::string& tag, double& out) const;

		js_result get_string(const std::string& tag, std::string& out) const;

		js_result get_array(const std::string& tag, array*& out) const;

		object* add_object(js_root* root, const std::string& tag);

		void add_bool(js_root* root, const std::string& tag, bool x);

		void add_number(js_root* pool, const std::string& tag, double x);

		void add_string(js_root* pool, const std::string& tag, const std::string& x);

		array* add_array(js_root* pool, const std::string& tag);

		void add(const std::string& tag, entry* data);

	private:
		std::unordered_map<std::string, entry*> map;
	};
}


#endif //JSON_JS_OBJECT_HPP
