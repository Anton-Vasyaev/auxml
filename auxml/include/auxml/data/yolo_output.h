#pragma once

// std
#include <cstdint>
// project
#include "output_tensor.h"
#include "yolo_layer_params.h"

struct yolo_output
{
    output_tensor output;

    yolo_layer_params params;
};