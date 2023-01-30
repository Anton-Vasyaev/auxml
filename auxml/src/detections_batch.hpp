#pragma once

// std
#include <vector>
// project
#include <auxml/data/object_detection.h>

namespace auxml
{

using detections_type = std::vector<object_detection>;

using detections_batch_type = std::vector<detections_type>;

}