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

extern "C" CALLISTO_EXPORT return_status CALLISTO_PYTHON_CALL
process_darknet_sealed_output_detections(
    float*          darknet_output,
    int64_t         boxes_count,
    int64_t         batch_size,
    int32_t         classes_count,
    float           object_threshold,
    float           nms_threshold,
    object_handler* detections_batch_handler // RETURN
);