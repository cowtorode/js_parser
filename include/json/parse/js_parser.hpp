#ifndef JSON_JS_PARSER_HPP
#define JSON_JS_PARSER_HPP


#include <stack>
#include "json/types/document/js_object_document.hpp"
#include "json/types/document/js_array_document.hpp"
#include "json/io/file.hpp"

namespace json
{
	enum error_code : unsigned int
	{
		OK = 0,
		INVALID_OBJECT_ENTRY_SYNTAX,
		INVALID_ARRAY_ENTRY_SYNTAX,
		INVALID_NUMBER_DIGIT,
		EXPECTED_ULL,
		EXPECTED_RUE,
		EXPECTED_ALSE,
		IO_EXCEPTION,
		OVERFLOW
	};

	const char* code_str(error_code code);

	class parser
	{
	public:
		explicit parser();

		bool parse_into(array_document& out, const std::string& json);

		bool parse_into(object_document& out, const std::string& json);

		bool parse_into(array_document& out, const file& json);

		bool parse_into(object_document& out, const file& json);

		[[nodiscard]] error_code code() const;

		void print_code() const;

	private:
		// state methods

		/**
		 * @return The number of bytes that are buffered but unread, computed as end - cursor.
		 */
		[[nodiscard]] inline long bytes_remaining() const;

		[[nodiscard]] inline object* top_object() const;

		[[nodiscard]] inline array* top_array() const;

		void skip_char();

		inline char peek_char();

		char read_char();

		/**
		 * @return The first nonwhitespace char or 'relevant' char at or after cursor
		 */
		char read_relevant_char();

		/**
		 * Functions the same as read_relevant_char() but doesn't increment past the char.
		 * This is used for the parse_into methods to not advance past the first char if
		 * the specified parse pattern is not matched (so if the API user tries to parse
		 * an array but an object is what the json format is in, you don't advance past
		 * the '{' so that you can still read the object if you so choose).
		 * @return The first nonwhitespace char or 'relevant' char at or after cursor
		 */
		char peek_relevant_char();

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

		double read_decimal();

		double read_number();

		/**
		 * Read an object, number, string, boolean, null, or array, and write it to the specified label.
		 */
		void parse_object_entry_data(const std::string& label);

		void parse_object();

		void parse_array();

		error_code err;
		const char* cursor;
		const char* end;
		// only ever an array_document or object_document
		std::stack<entry*> parse_stack;
		MemoryPool* pool;
	};
}


#endif //JSON_JS_PARSER_HPP