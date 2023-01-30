// parent header
#include "yolo_layer_detections.hpp"
// std
#include <cmath>
#include <exception>
#include <algorithm>

namespace auxml
{

using yolo_box_list_t = std::vector<float>;

using yolo_box_list_batch_t = std::vector<yolo_box_list_t>;

struct bbox
{
    float x1;
    float y1;
    float x2;
    float y2;

    inline float area() noexcept
    {
        return (x2 - x1) * (y2 - y1);
    }

    inline float iou(bbox other)
    {
        auto l = std::max(this->x1, other.x1);
        auto r = std::min(this->x2, other.x2);
        auto t = std::max(this->y1, other.y1);
        auto b = std::min(this->y2, other.y2);

        if (l >= r || t >= b) return 0.0f;

        auto self_area = area();
        auto other_area = other.area();

        auto inter_bbox = bbox {l, t, r, b};
        auto inter_area = inter_bbox.area();

        auto union_area = self_area + other_area - inter_area;

        return inter_area / union_area;
    }
};

inline float sigmoid(float value)
{
    return 1.0f / (1.0f + std::exp(-value));
}

inline float sigmoid_scale_x_y(float value, float scale_x_y)
{
    return sigmoid(value) * scale_x_y - 0.5f * (scale_x_y - 1.0f);
}


void activate_yolo_output(
    output_tensor batch_yolo_output,
    int32_t mask_count,
    int32_t classes_count,
    float scale_x_y
)
{
    auto b = batch_yolo_output.batch;
    auto h = batch_yolo_output.height;
    auto w = batch_yolo_output.width;
    auto c = batch_yolo_output.channels;

    auto mask_size    = 5 + classes_count;
    auto filters_size = mask_count * mask_size;

    float* output_cursor = batch_yolo_output.data;

    auto total_masks = b * h * w * mask_count;

    for(size_t mask_i = 0; mask_i < total_masks; mask_i++)
    {
        // activate and scale x, y 
        output_cursor[0] = sigmoid_scale_x_y(output_cursor[0], scale_x_y);
        output_cursor[1] = sigmoid_scale_x_y(output_cursor[1], scale_x_y);

        for(size_t activate_i = 4; activate_i < 5 + classes_count; activate_i++)
        {
            output_cursor[activate_i] = sigmoid(output_cursor[activate_i]);
        }

        output_cursor += mask_size;
    }
}

yolo_box_list_batch_t get_yolo_boxes_standard(
    std::vector<yolo_output>& output_layers,
    int32_t classes_count,
    int32_t net_width,
    int32_t net_height,
    float obj_conf_threshold
)
{
    auto mask_size = (5 + classes_count);

    auto batch_size = output_layers[0].output.batch;

    // allocate batch and fill
    yolo_box_list_batch_t boxes_batch;
    boxes_batch.reserve(batch_size);
    int64_t boxes_count = 0;
    for(auto& output_layer : output_layers)
    {
        auto tensor = output_layer.output;

        auto out_c = tensor.channels;
        auto out_w = tensor.width;
        auto out_h = tensor.height;

        auto current_boxes_count = (out_w + out_h + out_c);
        boxes_count += current_boxes_count;
    }

    for(int b_i = 0; b_i < batch_size; b_i++)
    {
        yolo_box_list_t box_list;
        box_list.reserve(boxes_count);
        boxes_batch.push_back(std::move(box_list));
    }

    for(auto& output_layer : output_layers)
    {
        auto anchors = output_layer.params.anchors;
        auto masks   = output_layer.params.mask;

        auto tensor = output_layer.output;

        auto out_c = tensor.channels;
        auto out_w = tensor.width;
        auto out_h = tensor.height;

        // Because in inner cycle has continue condition
        auto batch_cursor = tensor.data - mask_size;

        for(size_t b_i = 0; b_i < batch_size; b_i++)
        {
            auto& current_box_list = boxes_batch[b_i];

            for(size_t h_i = 0; h_i < out_h; h_i++)
            {
                for(size_t w_i = 0; w_i < out_w; w_i++)
                {
                    for(size_t mask_i = 0; mask_i < masks.size; mask_i++)
                    {
                        batch_cursor += mask_size;

                        auto anchor_idx = masks.data[mask_i];
                        auto curr_anchor = anchors.data[anchor_idx];

                        auto w_anchor = curr_anchor.x;
                        auto h_anchor = curr_anchor.y;

                        auto x        = batch_cursor[0];
                        auto y        = batch_cursor[1];
                        auto w        = batch_cursor[2];
                        auto h        = batch_cursor[3];
                        auto obj_conf = batch_cursor[4];

                        // objc_conf < obj_conf_threshold incorrect behavior for NaN values
                        if(!(obj_conf > obj_conf_threshold)) 
                        {
                            continue;
                        }

                        for(size_t class_i = 5; class_i < mask_size; class_i++)
                        {
                            auto class_prob = batch_cursor[class_i];
                            auto prob = obj_conf * class_prob;
                            batch_cursor[class_i] = prob > obj_conf_threshold ? prob : 0.0f;
                        }

                        x = (w_i + x) / out_w;
                        y = (h_i + y) / out_h;

                        w = std::exp(w) * w_anchor / net_width;
                        h = std::exp(h) * h_anchor / net_height;

                        auto x1 = x - w / 2.0f;
                        auto y1 = y - h / 2.0f;

                        auto x2 = x + w / 2.0f;
                        auto y2 = y + h / 2.0f;

                        current_box_list.push_back(x1);
                        current_box_list.push_back(y1);
                        current_box_list.push_back(x2);
                        current_box_list.push_back(y2);
                        current_box_list.push_back(obj_conf);

                        for(size_t class_i = 5; class_i < mask_size; class_i++)
                        {
                            auto class_prob = batch_cursor[class_i];
                            current_box_list.push_back(class_prob);
                        }
                    }
                }
            }
        }
    }

    return boxes_batch;
}

yolo_box_list_batch_t get_yolo_boxes(
    std::vector<yolo_output>& output_layers,
    int32_t classes_count,
    int32_t net_width,
    int32_t net_height,
    float obj_conf_threshold
)
{
    // validate layers
    for(auto& output_layer : output_layers)
    {
        if(output_layer.params.new_coords)
        {
            throw std::exception("Not implemented get_yolo_boxes for new coord");
        }
    }

    return get_yolo_boxes_standard(
        output_layers,
        classes_count,
        net_width,
        net_height,
        obj_conf_threshold
    );
}


void descending_sort_yolo_boxes(
    yolo_box_list_t& boxes,
    int32_t classes_count,
    int32_t class_sort_idx
)
{
    auto mask_size = 5 + classes_count;

    auto boxes_count = boxes.size() / mask_size;

    // NEED optimize sorting in future 
    // (write iterator or class wrapping around float array and present box structure and call std::sort)
    for(size_t i = 0; i < boxes_count - 1; i++)
    {
        for(size_t k = 0; k < boxes_count - i - 1; k++)
        {
            auto left_box_cursor = boxes.data() + k * mask_size;
            auto right_box_cursor = boxes.data() + (k + 1) * mask_size;
            auto left_prob = left_box_cursor[5 + class_sort_idx];
            auto right_prob = right_box_cursor[5 + class_sort_idx];

            if(left_prob < right_prob)
            {
                std::swap_ranges(left_box_cursor, left_box_cursor + mask_size, right_box_cursor);
            }
        }
    }
}

void do_nms_sort(
    yolo_box_list_batch_t& boxes_batch,
    int32_t classes_count,
    float nms_threshold 
)
{
    auto mask_size = 5 + classes_count;
    for(auto& yolo_boxes : boxes_batch)
    {
        if(yolo_boxes.size() == 0) continue;

        for(int32_t class_idx = 0; class_idx < classes_count; class_idx++)
        {
            descending_sort_yolo_boxes(yolo_boxes, classes_count, class_idx);

            auto boxes_count = yolo_boxes.size() / mask_size;

            for(size_t box_i = 0; box_i < boxes_count; box_i++)
            {
                auto main_cursor = yolo_boxes.data() + box_i * mask_size;
                auto main_prob   = main_cursor[5 + class_idx];

                if (main_prob == 0.0f)
                {
                    continue;
                }
                
                auto main_box = bbox {
                    main_cursor[0],
                    main_cursor[1],
                    main_cursor[2],
                    main_cursor[3]
                };

                for(size_t box_j = box_i + 1; box_j < boxes_count; box_j++)
                {
                    auto curr_cursor = yolo_boxes.data() + box_j * mask_size;
                    auto curr_prob = curr_cursor[5 + class_idx];
                    if(curr_prob == 0.0)
                    {
                        continue;
                    }

                    auto curr_box = bbox {
                        curr_cursor[0],
                        curr_cursor[1],
                        curr_cursor[2],
                        curr_cursor[3]
                    };

                    if(main_box.iou(curr_box) >= nms_threshold)
                    {
                        curr_cursor[5 + class_idx] = 0.0f;
                    }
                }
            }
        }
    }
}

detections_batch_type process_yolo_detections(
    yolo_output* output_layers,
    uint64_t outputs_count,
    net_params params,
    float object_threshold,
    float nms_threshold
)
{
    auto classes_count = output_layers[0].params.classes;
    auto mask_size     = 5 + classes_count;

    // Copy output values
    std::vector<std::vector<float>> copy_output_tensors_data;
    std::vector<yolo_output> copy_output_layers;

    copy_output_tensors_data.reserve(outputs_count);
    copy_output_layers.reserve(outputs_count);

    for(size_t i = 0; i < outputs_count; i++)
    {
        auto output_layer = output_layers[i];
        auto tensor = output_layer.output;

        auto total_size = tensor.batch * tensor.height * tensor.width * tensor.channels;

        auto copy_data = std::vector<float>(total_size);
        std::copy_n(tensor.data, total_size, copy_data.data());

        auto copy_output_layer = output_layers[i];
        copy_output_layer.output.data = copy_data.data();

        copy_output_tensors_data.push_back(std::move(copy_data));
        copy_output_layers.push_back(copy_output_layer);
    }

    // activate output tensors
    for(size_t i = 0; i < outputs_count; i++)
    {
        auto output_layer = copy_output_layers[i];
        auto params = output_layer.params;

        activate_yolo_output(output_layer.output, params.mask.size, params.classes, params.scale_x_y);
    }

    // form boxes data
    auto boxes_batch = get_yolo_boxes(copy_output_layers, classes_count, params.width, params.height, object_threshold);

    // non maximum supression processing
    do_nms_sort(boxes_batch, classes_count, nms_threshold);

    detections_batch_type object_detection_batch;

    for(auto& boxes_list : boxes_batch)
    {
        auto yolo_boxes_count = boxes_list.size() / mask_size;

        detections_type detections;
        detections.reserve(yolo_boxes_count);

        for(size_t i = 0; i < yolo_boxes_count; i++)
        {
            auto box_cursor = boxes_list.data() + i * mask_size;

            auto x1 = box_cursor[0];
            auto y1 = box_cursor[1];
            auto x2 = box_cursor[2];
            auto y2 = box_cursor[3];

            auto obj_conf = box_cursor[4];

            // find class with max prob
            float max_class_prob = box_cursor[5];
            int max_class_idx  = 5;
            for(size_t class_i = 5; class_i < mask_size; class_i++)
            {
                auto current_prob = box_cursor[class_i];
                if(current_prob > max_class_prob)
                {
                    max_class_prob = current_prob;
                    max_class_idx  = class_i;
                }
                max_class_prob = std::max(max_class_prob, box_cursor[class_i]);
            }
            
            if(max_class_prob != 0.0)
            {
                detections.push_back({x1, y1, x2, y2, max_class_idx - 5, obj_conf, max_class_prob});
            }
            box_cursor += mask_size;
        }

        detections.shrink_to_fit();
        object_detection_batch.push_back(std::move(detections));
    }

    return object_detection_batch;
}

}