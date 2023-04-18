#pragma once

#include <cmath>
// project
#include <callisto/math/primitives.hpp>
#include <callisto/math/functions.hpp>

namespace callisto::math
{

struct bbox2op_can
{
    template<typename bbox1_type, typename bbox2_type>
    inline static constexpr double iou(const bbox1_type& box1, const bbox2_type& box2)
    {
        auto l = max(box1.x1, box2.x1);
        auto r = min(box1.x2, box2.x2);
        auto t = max(box1.y1, box2.y1);
        auto b = min(box1.y2, box2.y2);

        if (l >= r || t >= b) return 0.0;

        auto area1 = box1.area();
        auto area2 = box2.area();

        auto inter_bbox = bbox2(l, t, r, b);
        auto inter_area = inter_bbox.area();

        auto union_area = area1 + area2 - inter_area;

        return inter_area / union_area;
    }

    template<typename bbox1_type, typename bbox2_type>
    inline static constexpr double iom(const bbox1_type& box1, const bbox2_type& box2)
    {
        auto l = max(box1.x1, box2.x1);
        auto r = min(box1.x2, box2.x2);
        auto t = max(box1.y1, box2.y1);
        auto b = min(box1.y2, box2.y2);

        if (l >= r || t >= b) return 0.0;

        auto area1 = box1.area();
        auto area2 = box2.area();

        auto min_area = min(area1, area2);

        auto inter_bbox = bbox2(l, t, r, b);
        auto inter_area = inter_bbox.area();

        return inter_area / min_area;
    }
};

} // namespace callisto::math