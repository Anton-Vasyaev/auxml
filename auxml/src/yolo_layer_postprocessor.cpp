/*
// parent header
#include <yolo_layer_postprocessing.hpp>


namespace auxml
{

#pragma region private_methods

void yolo_layer_postprocessing::move_from(yolo_layer_postprocessor& other)
{
    _net_params           = other._net_params;
    _yolo_output_params   = std::move(other._yolo_output_params)
    _boxes_predictions    = std::move(other._boxes_predictions);
    _output_tensor_cache = std::move(other._output_tensor_cache);
    _detections           = std::move(other._detections);
}

void yolo_layer_postprocessing::reset_cache(b_b::span<output_tensor> output_tensors)
{
}

#pragma endregion

#pragma region construct_and_destruct

yolo_layer_postprocessing::yolo_layer_postprocessing() {}

yolo_layer_postprocessing::yolo_layer_postprocessor(
    net_params net_params, 
    b_b::span<yolo_layer_params> yolo_output_params,
    b_b::span<output_tensor> output_tensors,
    int32_t start_batch_size
)
{
    _net_params = net_params;

    _yolo_output_params.resize(yolo_output_params.size());
    std::copy_n(yolo_output_params.data(), yolo_output_params.size(), _yolo_output_params.data());

    int64_t boxes_values_count = 0;
    for(auto& out_tensor : output_tensors)
    {
        auto h = out_tensor.height;
        auto w = out_tensor.width;
        auto c = out_tensor.channels;

        auto out_size = start_batch_size * h * w * c;
        
        auto cache_tensor = std::vector<float>();
        cache_tensor.reserve(out_size);

        _output_tensor_cache.push_back(std::move(cache_tensor));
    }
}

    yolo_layer_postprocessor(yolo_layer_postprocessor&& other);


    // methods
    void process(b_b::span<output_tensor> output_tensors);

    uint64_t get_batch_size() const;

    uint64_t detections_count(uint64_t batch_idx) const;

    void store_detections(object_detection* detections_ptr) const;

    // operators
    const yolo_layer_postprocessor& operator=(yolo_layer_postprocessor&&);


}
*/