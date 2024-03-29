#pragma once

#include "volk.h"

namespace mvk {
    enum class PrimitiveTopology : unsigned int {
        POINT_LIST = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        LINE_LIST = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
        LINE_LIST_WITH_ADJACENCY = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
        LINE_STRIP = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        LINE_STRIP_WITH_ADJACENCY = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
        PATCH_LIST = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST,
        TRIANGLE_LIST = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        TRIANGLE_LIST_WITH_ADJACENCY = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
        TRIANGLE_STRIP = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        TRIANGLE_STRIP_WITH_ADJACENCY = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
        TRIANGLE_FAN = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
    };
}
