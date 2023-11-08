// parent header
#include <yolo_sealed_output_detections.hpp>
// project
#include <data/darknet_box_provider.hpp>

#include <auxiliary/framework/collection.hpp>

namespace auxml
{

namespace c_f = callisto::framework;

using darknet_box_provider_list_type  = std::vector<darknet_box_provider>;
using darknet_box_provider_batch_type = std::vector<darknet_box_provider_list_type>;

void darknet_box_nms_sort(
    darknet_box_provider_batch_type& boxes_batch,
    int32_t                          classes_count,
    float*                           nms_thresholds
)
{
    for (size_t batch_i = 0; batch_i < boxes_batch.size(); batch_i++)
    {
        auto  current_nms_threshold = nms_thresholds[batch_i];
        auto& boxes_list            = boxes_batch[batch_i];

        for (int32_t class_i = 0; class_i < classes_count; class_i++)
        {
            std::sort(
                boxes_list.begin(),
                boxes_list.end(),
                [&, class_i](darknet_box_provider& first, darknet_box_provider& second)
                { return first.object_confidence() > second.object_confidence(); }
            );

            for (int64_t i = 0; i < boxes_list.size(); i++)
            {
                auto main_box  = boxes_list[i];
                auto main_prob = main_box.class_probabilities().data()[class_i];

                if (main_prob == 0.0) continue;

                for (int64_t j = i + 1; j < boxes_list.size(); j++)
                {
                    auto curr_box  = boxes_list[j];
                    auto curr_prob = curr_box.class_probabilities().data()[class_i];

                    if (curr_prob == 0.0) continue;

                    if (main_box.iou(curr_box) > current_nms_threshold)
                    {
                        curr_box.class_probabilities().data()[class_i] = 0.0;
                    }
                }
            }
        }
    }
}

detections_batch_type process_yolo_sealed_output_detections(
    float*  darknet_output,
    int64_t boxes_count,
    int64_t batch_size,
    int32_t classes_count,
    float*  object_thresholds,
    float*  nms_thresholds,
    int32_t net_width,
    int32_t net_height
)
{
    auto values_per_box_cell = 5 + classes_count;
    // alocate data
    auto box_batch = darknet_box_provider_batch_type();
    box_batch.reserve(batch_size);
    for (int64_t i = 0; i < batch_size; i++)
    {

        auto box_list = darknet_box_provider_list_type();
        box_list.reserve(boxes_count);
        box_batch.push_back(box_list);
    }

    int64_t box_offset         = 5 + classes_count;
    float*  yolo_output_cursor = darknet_output;
    for (int64_t batch_i = 0; batch_i < batch_size; batch_i++)
    {
        auto current_object_threshold = object_thresholds[batch_i];

        auto& current_box_list = box_batch[batch_i];
        for (int64_t box_i = 0; box_i < boxes_count; box_i++)
        {
            auto current_box = darknet_box_provider(yolo_output_cursor, classes_count);

            auto obj_conf = current_box.object_confidence();

            if (!(obj_conf > current_object_threshold))
            {
                yolo_output_cursor += values_per_box_cell;
                continue;
            }

            auto classes_probs = current_box.class_probabilities();
            for (int32_t class_i = 0; class_i < classes_count; class_i++)
            {
                auto class_prob = classes_probs.data()[class_i];
                auto prob       = obj_conf * class_prob;

                classes_probs.data()[class_i] = !(prob > current_object_threshold) ? 0.0 : prob;
            }

            current_box_list.push_back(current_box);

            yolo_output_cursor += values_per_box_cell;
        }
    }

    darknet_box_nms_sort(box_batch, classes_count, nms_thresholds);

    // fill detections
    auto detections_batch = detections_batch_type();
    detections_batch.reserve(batch_size);
    for (int64_t batch_i = 0; batch_i < batch_size; batch_i++)
    {
        auto detections_list = detections_type();
        detections_list.reserve(boxes_count);

        auto& darknet_boxes = box_batch[batch_i];

        for (auto& darknet_box : darknet_boxes)
        {
            auto probs      = darknet_box.class_probabilities();
            auto class_idx  = c_f::max_index(probs);
            auto class_prob = probs.data()[class_idx];
            if (class_prob != 0.0)
            {
                detections_list.push_back(object_detection {
                    .x1 = (darknet_box.x_c() - darknet_box.width() / 2.0f) / net_width,
                    .y1 = (darknet_box.y_c() - darknet_box.height() / 2.0f) / net_height,
                    .x2 = (darknet_box.x_c() + darknet_box.width() / 2.0f) / net_width,
                    .y2 = (darknet_box.y_c() + darknet_box.height() / 2.0f) / net_height,
                    .class_id = static_cast<int32_t>(class_idx),
                    .object_confidence = darknet_box.object_confidence(),
                    .class_confidence  = class_prob });
            }
        }
        detections_list.shrink_to_fit();
        detections_batch.push_back(std::move(detections_list));
    }

    return detections_batch;
}

} // namespace auxml