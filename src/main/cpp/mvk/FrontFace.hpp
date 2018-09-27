#pragma once

#include "volk.h"

namespace mvk {
    enum class FrontFace : unsigned int {
        COUNTER_CLOCKWISE = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        CLOCKWISE = VK_FRONT_FACE_CLOCKWISE
    };
}
