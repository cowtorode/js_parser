//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_ARRAY_DOCUMENT_HPP
#define JSON_JS_ARRAY_DOCUMENT_HPP


#include "js_document.hpp"
#include "types/js_array.hpp"

namespace json
{
	class array_document : public document, public array
	{
	public:
		array_document();

		explicit array_document(size_t size);

		[[nodiscard]] std::string json_string() const;
	};
}


#endif //JSON_JS_ARRAY_DOCUMENT_HPP