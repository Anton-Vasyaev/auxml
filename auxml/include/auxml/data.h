#pragma once

#include <cstdint>

struct alg_tuple2f
{
    float v1;
    
    float v2;
};

struct anchors_list
{
    alg_tuple2f* anchors;

    size_t anchors_count;
};