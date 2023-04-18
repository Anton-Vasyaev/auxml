// parent header
#include <auxml/api/darknet_sealed_output_detections_funcs.h>
// 3rd party
#include <callisto/framework/string.hpp>
#include <callisto/framework/exception.hpp>
// project
#include <auxml/data/object_type.h>

#include <darknet_sealed_output_detections.hpp>
#include <auxiliary/try_catch_api.hpp>
#include <auxiliary/object_handler_check.hpp>

namespace c_f = callisto::framework;

return_status process_darknet_sealed_output_detections(
    float*          darknet_output,
    int64_t         boxes_count,
    int64_t         batch_size,
    int32_t         classes_count,
    float           object_threshold,
    float           nms_threshold,
    object_handler* detections_batch_handler // RETURN
)
{
    API_TRY

    detections_batch_handler->object = nullptr;
    detections_batch_handler->type   = static_cast<int>(object_type::detections_batch);

    auto detections_uptr = std::make_unique<auxml::detections_batch_type>();

    auto detections_batch = auxml::process_darknet_sealed_output_detections(
        darknet_output,
        boxes_count,
        batch_size,
        classes_count,
        object_threshold,
        nms_threshold
    );

    *detections_uptr = std::move(detections_batch);

    detections_batch_handler->object = detections_uptr.release();

    API_CATCH
}