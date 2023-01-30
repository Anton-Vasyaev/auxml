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



extern "C" CALLISTO_EXPORT return_status CALLISTO_PYTHON_CALL detections_batch_size(
    object_handler detections_batch_handler,
    uint64_t* batch_size // RETURN
);

extern "C" CALLISTO_EXPORT return_status CALLISTO_PYTHON_CALL detections_batch_detections_count(
    object_handler detections_batch_handler,
    uint64_t batch_idx,
    uint64_t* detections_count // RETURN
);

extern "C" CALLISTO_EXPORT return_status CALLISTO_PYTHON_CALL detections_batch_detections_store(
    object_handler detections_batch_handler,
    uint64_t batch_idx,
    object_detection* detections
);