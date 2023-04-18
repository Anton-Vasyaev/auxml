#pragma once

// project
#include "detections_batch.hpp"
#include "auxml/data/yolo_output.h"
#include "auxml/data/net_params.h"

namespace auxml
{

detections_batch_type process_darknet_sealed_output_detections(
    float*  darknet_output,
    int64_t boxes_count,
    int64_t batch_size,
    int32_t classes_count,
    float   object_threshold,
    float   nms_threshold
);

}