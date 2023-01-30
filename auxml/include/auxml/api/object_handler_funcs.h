#pragma once

// 3rd party
#include <callisto/framework/native/lib_export.h>
// project
#include <auxml/data/object_handler.h>

#include "auxml/auxiliary/call.h"


extern "C" CALLISTO_EXPORT void CALLISTO_PYTHON_CALL object_handler_destroy(
    object_handler detections_batch_handler
);