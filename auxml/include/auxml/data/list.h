#pragma once

#include <cstdint>
// project
#include "anchor.h"

struct int_list
{
    int32_t* data;

    uint64_t size;
};

struct float_list
{
    float* data;
    uint64_t size;
};

struct anchor_list
{
    anchor* data;

    uint64_t size;
};