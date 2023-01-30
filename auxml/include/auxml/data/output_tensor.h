#pragma once

#include <cstdint>

struct output_tensor
{
    float* data;

    uint64_t batch;

    uint64_t width;

    uint64_t height;
    
    uint64_t channels;
};