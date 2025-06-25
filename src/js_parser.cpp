#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include "js_parser.hpp"
#include "js_root_object.hpp"
#include "js_root_array.hpp"
#include "types/js_number.hpp"
#include "types/js_bool.hpp"
#include "types/js_string.hpp"

JsonParser::JsonParser(const char* file) : file(file), fd(-1), cursor(buf), end(buf), parse_stack(), pool(nullptr)
{
    /* Don't need to initialize buf */
}

JsonParser::~JsonParser()
{
    close_file();
}

bool JsonParser::is_open() const
{
    return fd != -1;
}

void JsonParser::open_file()
{
    close_file();

    fd = open(file, 0, O_RDONLY);

    if (!is_open())
    {
        // Uh oh, the file didn't open
        perror("JsonParser::open_file(): open()");
    }
}

void JsonParser::close_file()
{
    if (is_open())
    {
        int res = close(fd);

        if (res == -1)
        {
            perror("JsonParser::close_file(): close()");
        } else
        {
            fd = -1;
        }
    }
}


js_object* JsonParser::top_object() const
{
    return (js_object*) parse_stack.top();
}

js_array* JsonParser::top_array() const
{
    return (js_array*) parse_stack.top();
}

long JsonParser::bytes_remaining() const
{
    return end - cursor;
}

void JsonParser::skip_char()
{
    ++cursor;
}

char JsonParser::read_char()
{
    if (bytes_remaining() < sizeof(char))
    {
        throw 0;
        // read more? What do we do here?
        return 0;
    }

    return *cursor++;
}

char JsonParser::skip_whitespace()
{
    while (true)
    {
        char c;

        switch (c = read_char())
        {
            case '\t': // Horizontal Tab (0x09)
            case '\n': // Line Feed (0x0A)
            case '\v': // Vertical Tab (0x0B)
            case '\f': // Form Feed (0x0C)
            case '\r': // Carriage Return (0x0D)
            case ' ':  // Space (0x20)
                // do nothing
                break;
            default:
                // we've expired all the whitespace chars, we can now return
                return c;
        }
    }
}

bool JsonParser::expect_string(const char* s)
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

std::string JsonParser::read_string(char u)
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

double JsonParser::read_double()
{
    return 1;
}

static js_bool* new_js_bool(MemoryPool* pool, bool x)
{
    return new (pool->palloc(sizeof(js_bool), alignof(js_bool))) js_bool(x);
}

static js_number* new_js_number(MemoryPool* pool, double x)
{
    return new (pool->palloc(sizeof(js_number), alignof(js_number))) js_number(x);
}

static js_string* new_js_string(MemoryPool* pool, const std::string& x)
{
    return new (pool->palloc(sizeof(js_string), alignof(js_string))) js_string(x);
}

static js_object* new_js_object(MemoryPool* pool)
{
    return new (pool->palloc(sizeof(js_object), alignof(js_object))) js_object;
}

static js_array* new_js_array(MemoryPool* pool)
{
    return new (pool->palloc(sizeof(js_array), alignof(js_array))) js_array;
}

void JsonParser::parse_object_entry_data(const std::string& label)
{
    // Similarly to parse_object(), the top of the parse_stack here is also
    // a subtype of js_object*, so top_object() is fine to access and push
    // data to. See parse_object for more context information.

    switch (skip_whitespace())
    {
        case 'n':
            if (expect_string("ull"))
            {
                top_object()->add(label, nullptr);
                break;
            }

            // except
            return;
        case 't':
            if (expect_string("rue"))
            {
                top_object()->add(label, new_js_bool(pool, true));
                break;
            }

            // except
            return;
        case 'f':
            if (expect_string("alse"))
            {
                top_object()->add(label, new_js_bool(pool, false));
                break;
            }

            // except
            return;
        case '.':
        case '-':
        case '+':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            top_object()->add(label, new_js_number(pool, read_double()));
            break;
        case '"':
            // "string"
            //  ^     ^
            //  start end
            top_object()->add(label, new_js_string(pool, read_string('"')));
            break;
        case '{':
            parse_stack.push(new_js_object(pool));

            parse_object();

            parse_stack.pop();
            break;
        case '[':
            parse_stack.push(new_js_array(pool));

            parse_array();

            parse_stack.pop();
            break;
        default:
            // todo illegal syntax
            return;
    }
}

void JsonParser::parse_object()
{
    /* This function is called when an opened curly bracket is encountered. After the
       call to read_char(), the cursor is thus placed one after the curly bracket:
       ...{...
           ^
       This function parses a javascript object, which is made up of a list of
       js_object entries enclosed between an opened and a closed curly brace {...}.
       Each js_object entry is formatted with a string label and a value of a varying data type
       ("label":"value"), and are separated by commas:

       {"number": 1, "string": "Chuck"}

       All white space is ignored.

       Valid js_object entry data types include:
       - null    "label": null
       - number  "label": 1        "label": 1.0   (serialized as a double)
       - boolean "label": true     "label": false
       - string  "label": "Norris"
       - object  "label": {}                      (nested objects may be populated with object entries)
       - array   "label": []                      (nested arrays may be populated with array entries)
       */

    /* This method assumes that the top of the parse_stack points to some type that's
       convertible to a js_object*. This allows us to treat the top of the parse_stack
       as a js_object* via top_object() in order to push js_object entries
       ("label":"value") to it. */

    // This gives us the first char of the first entry in the object.
    char c = skip_whitespace();

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
                if (skip_whitespace() != ':')
                {
                    // todo except
                    return;
                }

                // next we parse the data for the js_object entry and write it to the label on top_object()
                parse_object_entry_data(label);
                break;
            }
            default:
                // illegal syntax
                return;
        }

        c = skip_whitespace();

        // This switch case handles commas and object ending after entries finish.
        switch (c)
        {
            case '}': // end
                return;
            case ',': // expect an additional entry
                break;
            default: // illegal character
                // except
                return;
        }

        c = skip_whitespace();
    }
}

void JsonParser::parse_array()
{
    /* This function is called when an opened square bracket is encountered. After the
       call to read_char(), the cursor is thus placed one after the square bracket:
       ...[...
           ^
       This function parses a javascript array, which is made up of a list of
       js_array entries enclosed between an opened and a closed square brace [...].
       Each js_array entry is formatted by a value of a varying data type, and are
       separated by commas:

       [1, "Chuck"]

       All white space is ignored.

       Valid js_array entry data types include:
       - null    null
       - number  1        1.0   (serialized as a double)
       - boolean true     false
       - string  "Norris"
       - object  {}             (nested objects may be populated with object entries)
       - array   []             (nested arrays may be populated with array entries)
       */

    /* This method assumes that the top of the parse_stack points to some type that's
       convertible to a js_array*. This allows us to treat the top of the parse_stack
       as a js_array* via top_array() in order to push js_array entries to it. */

    // This gives us the first char of the first array element, (or ']' if it's empty)
    char c = skip_whitespace();

    while (true)
    {
        /* This switch statement parses array elements, handles immediate array termination ([]), but
           doesn't involve array element separation via commas. That job is left to the switch case
           after this one. */
        switch (c)
        {
            case ']': // empty array
                return;
            case 'n': // null
                if (expect_string("ull"))
                {
                    top_array()->add(nullptr);
                } else
                {
                    // except
                }
                break;
            case '.':
            case '-':
            case '+':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': // double
                top_array()->add(new_js_number(pool, read_double()));
                break;
            case 't': // boolean true
                // todo lowercase json
                if (expect_string("rue"))
                {
                    top_array()->add(new_js_bool(pool, true));
                }
                break;
            case 'f': // boolean false
                if (expect_string("alse"))
                {
                    top_array()->add(new_js_bool(pool, false));
                }
                break;
            case '"': // string
            {
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
                js_object* object = new_js_object(pool);
                top_array()->add(object);
                parse_stack.push(object);

                parse_object();

                parse_stack.pop();
                break;
            }
            case '[': // array
            {
                js_array* array = new_js_array(pool);
                top_array()->add(array);
                parse_stack.push(array);

                parse_array();

                parse_stack.pop();
                break;
            }
            default:
                // invalid character
                return;
        }

        // This gives us the first char after each array element, which parsing is left to this next switch case.
        c = skip_whitespace();

        // In all cases, the chars after each array element can only either be a comma, or the end of the array.
        switch (c)
        {
            case ']': // end
                return;
            case ',': // continue
                break;
            default: // invalid character
                return;
        }

        // If not returned, this gives us the first char of the next array element
        c = skip_whitespace();
    }
}

js_data* JsonParser::parse()
{
    res = read(fd, buf, sizeof(buf));

    if (res == -1)
    {
        perror("JsonParser::parse(): read()");
        return nullptr;
    }

    end = buf + res;

    // switch the first nonwhitespace character in the file.
    switch (skip_whitespace())
    {
        case '{':
        {
            js_root_object* root = new js_root_object(1024);
            parse_stack.push(root);
            pool = root->get_pool();

            parse_object();

            /* We can return the value inserted into the parse stack, but I just store it on the stack to be sure that
               what we heap allocated is what's returned, because if for any reason the js_root_object wasn't on the
               top of the stack by the time we got here, we'd be returning dirt memory. */
            return root;
        }
        case '[':
        {
            js_root_array* root = new js_root_array(1024);
            pool = root->get_pool();
            parse_stack.push(root);

            parse_array();

            //not needed since this the parse_stack isn't used after this for this same parse
            //parse_stack.pop();

            return root;
        }
        default:
            // invalid character
            // todo whenever there's an error we need to delete the root
            return nullptr;
    }
}
