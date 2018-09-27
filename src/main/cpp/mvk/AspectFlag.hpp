#pragma once

#include "volk.h"

namespace mvk {
    enum class AspectFlag : unsigned int {
        COLOR = VK_IMAGE_ASPECT_COLOR_BIT,
        DEPTH = VK_IMAGE_ASPECT_DEPTH_BIT,
        STENCIL = VK_IMAGE_ASPECT_STENCIL_BIT
    };
}
