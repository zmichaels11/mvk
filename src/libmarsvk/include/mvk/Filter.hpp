#pragma once

#include "volk.h"

namespace mvk {
    //! Sampler filter modes.
    enum class Filter : unsigned int {
        NEAREST = VK_FILTER_NEAREST,        /*!< Use Nearest-Neighbor filtering algorithm when sampling */
        LINEAR = VK_FILTER_LINEAR           /*!< Use Bilinear filtering algorithm when sampling. */
    };
}
