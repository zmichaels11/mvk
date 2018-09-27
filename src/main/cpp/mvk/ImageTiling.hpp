#pragma once

#include "volk.h"

namespace mvk {
    enum class ImageTiling : unsigned int {
        OPTIMAL = VK_IMAGE_TILING_OPTIMAL,
        LINEAR = VK_IMAGE_TILING_LINEAR
    };
}
