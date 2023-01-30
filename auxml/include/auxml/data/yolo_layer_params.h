#pragma once

// std
#include <cstdint>
// project
#include "list.h"

struct yolo_layer_params
{
    int32_t classes;

    int32_t total;

    int_list mask;

    int32_t max_boxes;

    float scale_x_y;

    int32_t new_coords; // bool original

    float iou_normalizer;

    float obj_normalizer;

    float cls_normalizer;

    float delta_normalizer;

    int32_t iou_loss; // IoULossType

    int32_t iou_thresh_kind; // IoULossType

    float beta_nms;

    int32_t nms_kind; // NmsKindType

    float jitter;

    float resize;

    float ignore_thresh;

    float truth_thresh;

    float iou_thresh;

    float random;

    anchor_list anchors;
};