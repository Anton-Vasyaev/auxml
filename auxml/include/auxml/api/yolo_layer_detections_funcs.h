#pragma once

// 3rd party
#include <callisto/framework/native/lib_export.h>
// project
#include "auxml/data/object_handler.h"
#include "auxml/data/return_status.h"
#include "auxml/data/yolo_output.h"
#include "auxml/data/object_detection.h"
#include "auxml/data/net_params.h"

#include "auxml/auxiliary/call.h"

extern "C" CALLISTO_EXPORT return_status CALLISTO_PYTHON_CALL process_yolo_detections(
    yolo_output* output_layers, // output type detections_batch_type
    uint64_t outputs_count,
    net_params params,
    float object_threshold,
    float nms_threshold,
    object_handler* detections_batch_handler // RETURN
);