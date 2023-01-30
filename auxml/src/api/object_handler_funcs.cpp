// parent header
#include <auxml/api/object_handler_funcs.h>
// project
#include <auxml/data/object_type.h>

#include <detections_batch.hpp>


void CALLISTO_PYTHON_CALL object_handler_destroy(
    object_handler detections_batch_handler
)
{
    auto object_ptr = detections_batch_handler.object;
    if(object_ptr == nullptr)
    {
        return;
    }

    auto ob_type = static_cast<object_type>(detections_batch_handler.type);
    switch(ob_type)
    {
        case object_type::detections_batch:
        {
            auto batch_ptr = (auxml::detections_batch_type*)object_ptr;
            delete batch_ptr;
            break;
        }
        default:
            return;
    }
}