#ifndef JSON_JS_PARSER_HPP
#define JSON_JS_PARSER_HPP


#include <stack>
#include "types/js_entry.hpp"
#include "types/js_object.hpp"
#include "memory_pool.hpp"
#include "io/file.hpp"

namespace json
{
	class parser
	{
	public:
		explicit parser();

		entry* parse(const std::string& json);

		entry* parse(const file& json);

	private:
		// state methods

		/**
		 * @return The number of bytes that are buffered but unread, computed as end - cursor.
		 */
		[[nodiscard]] inline long bytes_remaining() const;

		[[nodiscard]] inline object* top_object() const;

		[[nodiscard]] inline array* top_array() const;

		char read_char();

		/**
		 * @return The first nonwhitespace char including and after cursor
		 */
		char skip_whitespace();

		/**
		 * Asserts that the following characters in the stream match the string `s`,
		 * assuming the first character has already been read and matched.
		 *
		 * For example, after encountering 'n', call assert_token("ull") to verify "null".
		 * @return True if the immediate sequence of chars starting at and including cursor are equivalent to
		 *         the string of chars passed to this method, and false if there exists a character mismatch.
		 */
		bool expect_string(const char* s);

		/**
		 * Reads a string until the specified character exclusive.
		 * "hello"
		 *  ^
		 *  cursor
		 * read_string('"') will return "hello"
		 * @return The string starting at and including cursor until the passed character, exclusive.
		 */
		std::string read_string(char until);

		double read_double();

		/**
		 * Read an object, number, string, boolean, null, or array, and write it to the specified label.
		 */
		void parse_object_entry_data(const std::string& label);

		void parse_object();

		void parse_array();

		entry* parse();

		const char* cursor;
		const char* end;
		// only ever a root_array or root_object
		std::stack<entry*> parse_stack;
		MemoryPool* pool;
	};
}


#endif //JSON_JS_PARSER_HPP