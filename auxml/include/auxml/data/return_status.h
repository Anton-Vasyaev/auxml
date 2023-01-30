#pragma once

// std
#include <cstdint>

struct return_status
{
    int32_t correct_status;

    char* error_message;

    int32_t is_const_message;
};
