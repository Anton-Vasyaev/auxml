#pragma once

// project
#include "detections_batch.hpp"
#include "auxml/data/yolo_output.h"
#include "auxml/data/net_params.h"

namespace auxml
{

detections_batch_type process_yolo_sealed_output_detections(
    float*  darknet_output,
    int64_t boxes_count,
    int64_t batch_size,
    int32_t classes_count,
    float*  object_thresholds,
    float*  nms_thresholds,
    int32_t net_width,
    int32_t net_height
);

}