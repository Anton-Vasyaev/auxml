#pragma once

// 3rd party
#include <callisto/framework/string.hpp>
#include <callisto/framework/exception.hpp>
// project
#include "auxml/data/object_handler.h"
#include "auxml/data/object_type.h"

namespace c_f = callisto::framework;


inline void check_object_handler_type(object_handler handler, object_type expected_type)
{
    auto handler_type = static_cast<object_type>(handler.type);
    if(handler_type != expected_type)
    {
        throw c_f::runtime_exception()
            << c_f::error_tag_message(
                c_f::_bs(
                    "Failed to check object_handler type, expected:", 
                    object_type_str(expected_type),
                    ", got:", object_type_str(handler_type)
                )
            );
    }
}

template<typename type>
inline type& cast_object_handler(object_handler handler)
{
    return reinterpret_cast<type&>(*((type*)(handler.object)));
}