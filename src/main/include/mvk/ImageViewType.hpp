#pragma once

#include "volk.h"

namespace mvk {
    enum class ImageViewType : unsigned int {
        VIEW_1D = VK_IMAGE_VIEW_TYPE_1D,
        VIEW_2D = VK_IMAGE_VIEW_TYPE_2D,
        VIEW_3D = VK_IMAGE_VIEW_TYPE_3D,
        VIEW_1D_ARRAY = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
        VIEW_2D_ARRAY = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
        VIEW_CUBE = VK_IMAGE_VIEW_TYPE_CUBE,
        VIEW_CUBE_ARRAY = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
    };
}
