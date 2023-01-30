#pragma once

// std
#include <cstdint>


enum class object_type : int32_t
{
    unknown,
    detections_batch
};

constexpr const char* object_type_str(object_type type)
{
    switch(type)
    {
        case object_type::detections_batch: return "detections_batch";
        default : return "unknown";
    }    
}