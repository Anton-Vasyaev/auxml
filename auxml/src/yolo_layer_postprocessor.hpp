#pragma once

/*

// std
#include <vector>
// 3rd party
#include <boost/beast/core/span.hpp>
// project
#include <auxml/data/net_params.h>
#include <auxml/data/yolo_layer_params.h>
#include <auxml/data/output_tensor.h>

#include "detections_batch.hpp"


namespace auxml
{
namespace
{
    namespace b_b = boost::beast;
}

class yolo_layer_postprocessor
{
    using yolo_box_predictions = std::vector<float>;
    
    using yolo_box_predictions_batch = std::vector<yolo_box_predictions>;

    net_params _net_params;

    std::vector<yolo_layer_params> _yolo_output_params;

    yolo_box_predictions_batch _boxes_predictions;

    std::vector<std::vector<float>> _output_tensor_cache;

    detections_batch_type _detections;

    // private methods
    void move_from(yolo_layer_postprocessor& other);

    void reset_cache(b_b::span<output_tensor> output_tensors);
public:
    // deleted functions
    yolo_layer_postprocessor(const yolo_layer_postprocessor&) = delete;
    
    const yolo_layer_postprocessor& operator=(const yolo_layer_postprocessor&) = delete;

    // construct and destruct
    yolo_layer_postprocessing()

    yolo_layer_postprocessor(
        net_params net_params, 
        b_b::span<yolo_layer_params> yolo_output_params,
        b_b::span<output_tensor> output_tensors,
        int32_t start_batch_size
    );

    yolo_layer_postprocessor(yolo_layer_postprocessor&& other);


    // methods
    void process(b_b::span<output_tensor> output_tensors);

    uint64_t get_batch_size() const;

    uint64_t detections_count(uint64_t batch_idx) const;

    void store_detections(object_detection* detections_ptr) const;

    // operators
    const yolo_layer_postprocessor& operator=(yolo_layer_postprocessor&&);
};
}
*/