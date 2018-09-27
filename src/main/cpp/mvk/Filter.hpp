#pragma once

#include "volk.h"

namespace mvk {
    enum class Filter : unsigned int {
        NEAREST = VK_FILTER_NEAREST,
        LINEAR = VK_FILTER_LINEAR
    };
}
