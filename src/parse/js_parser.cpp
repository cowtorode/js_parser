#include <iostream>
#include "parse/js_parser.hpp"
#include "types/js_number.hpp"
#include "types/js_bool.hpp"
#include "types/js_string.hpp"
#include "util/math.hpp"

namespace json
{
	const char* code_str(error_code code)
	{
		switch (code)
		{
			case OK: return nullptr;
			case INVALID_OBJECT_ENTRY_SYNTAX: return "invalid syntax for object entry";
			case INVALID_ARRAY_ENTRY_SYNTAX: return "invalid syntax for array entry";
			case INVALID_NUMBER_DIGIT: return "encountered nondigit in number";
			case EXPECTED_ULL: return "expected 'ull' to complete null token";
			case EXPECTED_RUE: return "expected 'rue' to complete true token";
			case EXPECTED_ALSE: return "expected 'alse' to complete false token";
			case IO_EXCEPTION: return "failed to read file";
			case OVERFLOW: return "unable to finish parsing due to incomplete json source";
		}
	}

	parser::parser() : err(OK), cursor(nullptr), end(nullptr), parse_stack(), pool(nullptr) {}

	error_code parser::code() const
	{
		return err;
	}

	void parser::print_code() const
	{
		//	std::string:line:char: error (code): expected 'alse' to complete 'false' boolean value
		//		"test":frue
		//			    ^

		//	example.json:line:char: error (code): ...

		if (err)
		{
			// todo Source
			std::cerr << "std::string:0:0: error (" << err << "): " << code_str(err);

			// fixme This probably shouldn't go here for cases when err isn't related to errno
			if (errno)
			{
				std::cerr << ": ";
				perror(nullptr);
			} else
			{
				std::cerr << std::endl;
			}

			std::cerr << '\t';

			for (int i = 0; i < 25; ++i)
			{
				std::cerr << cursor[i];
			}

			std::cerr << std::endl
			          << "\t^" << std::endl;
		}
	}

	object* parser::top_object() const
	{
		return (object*) parse_stack.top();
	}

	array* parser::top_array() const
	{
		return (array*) parse_stack.top();
	}

	long parser::bytes_remaining() const
	{
		return end - cursor;
	}

	void parser::skip_char()
	{
		if (bytes_remaining() < sizeof(char))
		{
			// todo if reading fails
			err = OVERFLOW;
		} else
		{
			++cursor;
		}
	}

	char parser::peek_char()
	{
		return *cursor;
	}

	char parser::read_char()
	{
		if (bytes_remaining() < sizeof(char))
		{
			// todo if reading fails
			err = OVERFLOW;
			return 0;
		}

		return *cursor++;
	}

	char parser::read_relevant_char()
	{
		while (true)
		{
			char c = read_char();

			if (is_relevant(c))
			{
				return c;
			}
		}
	}

	char parser::peek_relevant_char()
	{
		while (true)
		{
			char c = peek_char();

			if (is_relevant(c))
			{
				return c;
			}

			skip_char();
		}
	}

	bool parser::expect_string(const char* s)
	{
		do
		{
			if (!*s)
			{
				return true;
			}
		} while (read_char() == *s++);

		return false;
	}

	std::string parser::read_string(char u)
	{
		std::string rax;

		char c = read_char();

		while (c != u)
		{
			// while the current char is not the specified terminator character
			rax += c;
			c = read_char();
		}

		return rax;
	}

	double parser::read_decimal()
	{
		/* This places the cursor one after a '.' symbol:
		   .1415
		    ^ cursor
		   h = 0.1
		   x = (1 * h) + (4 * h * h) + (1 * h * h * h) + ... */

		double x = 0;
		char c;
		double decimal_place = INVERSE_10;

		// parses d, the decimal segment of the double
		while (true)
		{
			c = peek_char();

			if (!c)
			{
				return x;
			}

			if (is_digit(c))
			{
				x += (c - '0') * decimal_place;
				decimal_place *= INVERSE_10;
			} else
			{
				// number has ended, we need to parse the current char for other syntax
				return x;
			}

			skip_char();
		}
	}

	double parser::read_number()
	{
		double x = 0;
		char c;

		// This loop parses the integer segment of the double
		while (true)
		{
			c = peek_char();

			// todo refactor to err
			if (!c)
			{
				return x;
			}

			if (is_digit(c))
			{
				x *= 10;
				x += (c - '0');
			} else if (c == '.')
			{
				break; // goto decimal parsing
			} else
			{
				// number has ended, we need to parse the current char for other syntax
				return x;
			}

			skip_char();
		}

		x += read_decimal();

		return x;
	}

	static boolean* new_js_bool(MemoryPool* pool, bool x)
	{
		return new(pool->palloc(sizeof(boolean), alignof(boolean))) boolean(x);
	}

	static number* new_js_number(MemoryPool* pool, double x)
	{
		return new(pool->palloc(sizeof(number), alignof(number))) number(x);
	}

	static string* new_js_string(MemoryPool* pool, const std::string& x)
	{
		return new(pool->palloc(sizeof(string), alignof(string))) string(x);
	}

	static object* new_js_object(MemoryPool* pool)
	{
		return new(pool->palloc(sizeof(object), alignof(object))) object;
	}

	static array* new_js_array(MemoryPool* pool)
	{
		return new(pool->palloc(sizeof(array), alignof(array))) array;
	}

	void parser::parse_object_entry_data(const std::string& label)
	{
		// Similarly to parse_object(), the top of the parse_stack here is also
		// a subtype of object*, so top_object() is fine to access and push
		// data to. See parse_object for more context information.

		char c = peek_relevant_char();

		switch (c)
		{
			case 'n':
				skip_char();
				if (expect_string("ull"))
				{
					top_object()->add(label, nullptr);
					break;
				}

				err = EXPECTED_ULL;
				return;
			case 't':
				skip_char();
				if (expect_string("rue"))
				{
					top_object()->add(label, new_js_bool(pool, true));
					break;
				}

				err = EXPECTED_RUE;
				return;
			case 'f':
				skip_char();
				if (expect_string("alse"))
				{
					top_object()->add(label, new_js_bool(pool, false));
					break;
				}

				err = EXPECTED_ALSE;
				return;
			case '.':
				skip_char();
				top_object()->add(label, new_js_number(pool, read_decimal()));
				break;
			case '-':
				skip_char();
				top_object()->add(label, new_js_number(pool, -read_number()));
				break;
			case '+':
			case '0':
				skip_char();
				top_object()->add(label, new_js_number(pool, read_number()));
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				top_object()->add(label, new_js_number(pool, read_number()));
				break;
			case '"':
				skip_char();
				// "string"
				//  ^     ^
				//  start end
				top_object()->add(label, new_js_string(pool, read_string('"')));
				break;
			case '{':
				skip_char();
				parse_stack.push(new_js_object(pool));

				parse_object();

				parse_stack.pop();
				break;
			case '[':
				skip_char();
				parse_stack.push(new_js_array(pool));

				parse_array();

				parse_stack.pop();
				break;
			default:
				err = INVALID_OBJECT_ENTRY_SYNTAX;
				return;
		}
	}

	void parser::parse_object()
	{
		/* This function is called when an opened curly bracket is encountered. After the
		   call to read_char(), the cursor is thus placed one after the curly bracket:
		   ...{...
			   ^
		   This function parses a javascript object, which is made up of a list of
		   object entries enclosed between an opened and a closed curly brace {...}.
		   Each object entry is formatted with a string label and a value of a varying data type
		   ("label":"value"), and are separated by commas:

		   {"x": 1, "string": "Chuck"}

		   All white space is ignored.

		   Valid object entry data types include:
		   - null    "label": null
		   - number  "label": 1        "label": 1.0   (serialized as a double)
		   - boolean "label": true     "label": false
		   - string  "label": "Norris"
		   - object  "label": {}                      (nested objects may be populated with object entries)
		   - array   "label": []                      (nested arrays may be populated with array entries)
		   */

		/* This method assumes that the top of the parse_stack points to some type that's
		   convertible to a object*. This allows us to treat the top of the parse_stack
		   as a object* via top_object() in order to push object entries
		   ("label":"value") to it. */

		// This gives us the first char of the first entry in the object.
		char c = read_relevant_char();

		while (true)
		{
			// This switch statement is responsible for parsing object entries: "label":"value"
			/* Each entry must start with a string label, so the only allowed characters are '}' for an empty object,
			   or " to begin a label. */
			switch (c)
			{
				case '}': // If the first char of the first entry is an end brace, the object is empty, return
					return;
				case '"':
				{
					// "label" : "value"
					//  ^ cursor
					// read_string('"') returns label, leaving cursor at:
					// "label":"value"
					//        ^ cursor
					std::string label = read_string('"');

					// the next char after the label needs to be a colon
					if (read_relevant_char() != ':')
					{
						err = INVALID_OBJECT_ENTRY_SYNTAX;
						return;
					}

					// next we parse the data for the object entry and write it to the label on top_object()
					parse_object_entry_data(label);
					break;
				}
				default:
					// illegal syntax
					return;
			}

			c = read_relevant_char();

			// This switch case handles commas and object ending after entries finish.
			switch (c)
			{
				case '}': // end
					return;
				case ',': // expect an additional entry
					break;
				default:
					err = INVALID_OBJECT_ENTRY_SYNTAX;
					return;
			}

			c = read_relevant_char();
		}
	}

	void parser::parse_array()
	{
		/* This function is called when an opened square bracket is encountered. After the
		   call to read_char(), the cursor is thus placed one after the square bracket:
		   ...[...
			   ^
		   This function parses a javascript array, which is made up of a list of
		   array entries enclosed between an opened and a closed square brace [...].
		   Each array entry is formatted by a value of a varying data type, and are
		   separated by commas:

		   [1, "Chuck"]

		   All white space is ignored.

		   Valid array entry data types include:
		   - null    null
		   - number  1        1.0   (serialized as a double)
		   - boolean true     false
		   - string  "Norris"
		   - object  {}             (nested objects may be populated with object entries)
		   - array   []             (nested arrays may be populated with array entries)
		   */

		/* This method assumes that the top of the parse_stack points to some type that's
		   convertible to a array*. This allows us to treat the top of the parse_stack
		   as a array* via top_array() in order to push array entries to it. */

		// This gives us the first char of the first array element, (or ']' if it's empty)
		char c = peek_relevant_char();

		while (true)
		{
			/* This switch statement parses array elements, handles immediate array termination ([]), but
			   doesn't involve array element separation via commas. That job is left to the switch case
			   after this one. */
			switch (c)
			{
				case ']': // empty array
					skip_char();
					return;
				case 'n': // null
					skip_char();
					if (expect_string("ull"))
					{
						top_array()->add(nullptr);
					} else
					{
						err = EXPECTED_ULL;
					}
					break;
				case '.':
					skip_char();
					top_array()->add(new_js_number(pool, read_decimal()));
					break;
				case '-':
					skip_char();
					top_array()->add(new_js_number(pool, -read_number()));
					break;
				case '+':
				case '0':
					skip_char();
					top_array()->add(new_js_number(pool, read_number()));
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					top_array()->add(new_js_number(pool, read_number()));
					break;
				case 't': // boolean true
					skip_char();
					// todo lowercase json
					if (expect_string("rue"))
					{
						top_array()->add(new_js_bool(pool, true));
					}
					break;
				case 'f': // boolean false
					skip_char();
					if (expect_string("alse"))
					{
						top_array()->add(new_js_bool(pool, false));
					}
					break;
				case '"': // string
				{
					skip_char();
					// "string"
					//  ^     ^
					//  start end
					std::string str = read_string('"');
					// next read ignoring whitespace should either be a ',' or ']'

					top_array()->add(new_js_string(pool, str));
					break;
				}
				case '{': // object
				{
					skip_char();
					object* object = new_js_object(pool);
					top_array()->add(object);
					parse_stack.push(object);

					parse_object();

					parse_stack.pop();
					break;
				}
				case '[': // array
				{
					skip_char();
					array* array = new_js_array(pool);
					top_array()->add(array);
					parse_stack.push(array);

					parse_array();

					parse_stack.pop();
					break;
				}
				default:
					err = INVALID_ARRAY_ENTRY_SYNTAX;
					return;
			}

			// This gives us the first char after each array element, which parsing is left to this next switch case.
			c = read_relevant_char();

			// In all cases, the chars after each array element can only either be a comma, or the end of the array.
			switch (c)
			{
				case ']': // end
					return;
				case ',': // continue
					break;
				default:
					err = INVALID_ARRAY_ENTRY_SYNTAX;
					return;
			}

			// If not returned, this gives us the first char of the next array element
			c = peek_relevant_char();
		}
	}

	bool parser::parse_into(array_document& out, const std::string& json)
	{
		cursor = json.c_str();
		end = cursor + json.size();

		if (peek_relevant_char() != '[')
		{
			return false;
		}

		// skip the opening square bracket
		skip_char();

		pool = &out.pool;
		parse_stack.push(&out);

		parse_array();

		parse_stack.pop();

		return !err;
	}

	bool parser::parse_into(object_document& out, const std::string& json)
	{
		return false;
	}

	bool parser::parse_into(array_document& out, const file& json)
	{
		char buf[1024];

		// buffer file into parser
		ssize_t res = json.buffer(buf, sizeof(buf));

		if (res == -1)
		{
			err = IO_EXCEPTION;
			return false;
		}

		cursor = buf;
		end = buf + res;

		// begin parsing
		if (peek_relevant_char() != '[')
		{
			return false;
		}

		// skip the opening square bracket
		skip_char();

		pool = &out.pool;
		parse_stack.push(&out);

		parse_array();

		parse_stack.pop();

		return !err;
	}

	bool parser::parse_into(object_document& out, const file& json)
	{
		return false;
	}
}