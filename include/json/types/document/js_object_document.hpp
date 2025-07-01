//
// Created by cory on 6/24/25.
//

#ifndef JSON_JS_OBJECT_DOCUMENT_HPP
#define JSON_JS_OBJECT_DOCUMENT_HPP


#include "js_document.hpp"
#include "json/types/js_object.hpp"
#include "memory_pool.hpp"

namespace json
{
	class object_document : public document, public object
	{
	public:
		explicit object_document(size_t size);

		std::string json_string() const;
	};
}


#endif //JSON_JS_OBJECT_DOCUMENT_HPP