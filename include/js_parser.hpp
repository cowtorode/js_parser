#ifndef JSON_JS_PARSER_HPP
#define JSON_JS_PARSER_HPP


#include <stack>
#include "types/js_data.hpp"
#include "types/js_object.hpp"
#include "memory_pool.hpp"

class JsonParser
{
public:
    explicit JsonParser(const char* file);

    ~JsonParser();

    void open_file();

    void close_file();

    js_data* parse();
private:
    // state methods

    /**
     * @return The state of this object's file handle, whether opened or closed.
     */
    [[nodiscard]] inline bool is_open() const;

    /**
     * @return The number of bytes that are buffered but unread, computed as end - cursor.
     */
    [[nodiscard]] inline long bytes_remaining() const;

    [[nodiscard]] inline js_object* top_object() const;

    [[nodiscard]] inline js_array* top_array() const;

    /**
     * Skips a character by incrementing the cursor pointer.
     */
    inline void skip_char();

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

    const char* file;
    int fd;

    unsigned long long res;
    char buf[1024];
    char* cursor;
    char* end;
    // only ever a js_root_array or js_root_object
    std::stack<js_data*> parse_stack;
    MemoryPool* pool;
};


#endif //JSON_JS_PARSER_HPP
