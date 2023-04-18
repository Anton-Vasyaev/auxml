// parent header
#include <auxml/api/yolo_layer_detections_funcs.h>
// 3rd party
#include <callisto/framework/string.hpp>
#include <callisto/framework/exception.hpp>
// project
#include <auxml/data/object_type.h>

#include <yolo_layer_detections.hpp>
#include <auxiliary/try_catch_api.hpp>
#include <auxiliary/object_handler_check.hpp>

namespace c_f = callisto::framework;

return_status CALLISTO_PYTHON_CALL process_yolo_detections(
    yolo_output*    output_layers,
    uint64_t        outputs_count,
    net_params      params,
    float*          object_thresholds,
    float*          nms_thresholds,
    object_handler* detections_batch_handler // RETURN output type detections_batch_type
)
{
    API_TRY

    detections_batch_handler->object = nullptr;
    detections_batch_handler->type   = static_cast<int>(object_type::detections_batch);

    auto detections_uptr = std::make_unique<auxml::detections_batch_type>();

    auto detections_batch = auxml::process_yolo_detections(
        output_layers,
        outputs_count,
        params,
        object_thresholds,
        nms_thresholds
    );

    *detections_uptr = std::move(detections_batch);

    detections_batch_handler->object = detections_uptr.release();

    API_CATCH
}
