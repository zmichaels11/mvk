#pragma once

#include "volk.h"

namespace mvk {
    //! Surface ColorSpaces
    enum class ColorSpace : unsigned int {
        SRGB_NONLINEAR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  //! The presentation engine supports the sRGB color space.
    };
}
