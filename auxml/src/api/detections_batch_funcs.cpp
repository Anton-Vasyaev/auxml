// parent header
#include <auxml/api/detections_batch_funcs.h>
// project
#include <auxiliary/object_handler_check.hpp>
#include <auxiliary/try_catch_api.hpp>

#include <detections_batch.hpp>


void validate_batch_idx(uint64_t batch_size, uint64_t batch_idx)
{
    // usigned, below zero checking is absent
    if(batch_idx > batch_size)
    {
        throw c_f::runtime_exception()
            << c_f::error_tag_message(
                c_f::_bs("Invalid batch idx (batch size = ", batch_size, "):", batch_idx)
            );
    }
}



return_status CALLISTO_PYTHON_CALL detections_batch_size(
    object_handler detections_batch_handler,
    uint64_t* batch_size // RETURN
)
{
    API_TRY

    check_object_handler_type(detections_batch_handler, object_type::detections_batch);

    auto& detections_batch = cast_object_handler<auxml::detections_batch_type>(detections_batch_handler);

    *batch_size = detections_batch.size();

    API_CATCH
}

return_status CALLISTO_PYTHON_CALL detections_batch_detections_count(
    object_handler detections_batch_handler,
    uint64_t batch_idx,
    uint64_t* detections_count // RETURN
)
{
    API_TRY

    check_object_handler_type(detections_batch_handler, object_type::detections_batch);

    auto& detections_batch = cast_object_handler<auxml::detections_batch_type>(detections_batch_handler);

    validate_batch_idx(detections_batch.size(), batch_idx);

    *detections_count = detections_batch[batch_idx].size();

    API_CATCH
}

return_status CALLISTO_PYTHON_CALL detections_batch_detections_store(
    object_handler detections_batch_handler,
    uint64_t batch_idx,
    object_detection* detections
)
{
    API_TRY

    check_object_handler_type(detections_batch_handler, object_type::detections_batch);

    auto& detections_batch = cast_object_handler<auxml::detections_batch_type>(detections_batch_handler);

    validate_batch_idx(detections_batch.size(), batch_idx);

    auto& detections_vector = detections_batch[batch_idx];

    std::copy_n(
        detections_vector.data(), 
        detections_vector.size(), 
        detections
    );

    API_CATCH
}