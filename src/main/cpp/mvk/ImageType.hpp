#pragma once

#include "volk.h"

namespace mvk {
    enum class ImageType : unsigned int {
        IMAGE_1D = VK_IMAGE_TYPE_1D,
        IMAGE_2D = VK_IMAGE_TYPE_2D,
        IMAGE_3D = VK_IMAGE_TYPE_3D
    };
}
