#pragma once

#include "mvk/Extent2D.hpp"
#include "mvk/Offset2D.hpp"

namespace mvk {
    struct Rect2D {
        Offset2D offset;
        Extent2D extent;
    };
}
