#pragma once

// project
#include "detections_batch.hpp"
#include "auxml/data/yolo_output.h"
#include "auxml/data/net_params.h"

namespace auxml
{

detections_batch_type process_yolo_detections(
    yolo_output* output_layers,
    uint64_t outputs_count,
    net_params params,
    float object_threshold,
    float nms_threshold
);
}