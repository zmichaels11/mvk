#pragma once

#include "volk.h"

namespace mvk {
    enum class PolygonMode : unsigned int {
        FILL = VK_POLYGON_MODE_FILL,
        LINE = VK_POLYGON_MODE_LINE,
        POINT = VK_POLYGON_MODE_POINT
    };
}
