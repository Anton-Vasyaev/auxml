#pragma once

// std
#include <cstdint>

struct object_detection
{
    float x1;
    
    float y1;

    float x2;

    float y2;

    int32_t class_id;

    float object_confidence;

    float class_confidence;
};