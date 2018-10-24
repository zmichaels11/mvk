#pragma once

#include "volk.h"

namespace mvk {
    //! ImageTiling specifies how subsections of an Image are stored in memory.
    enum class ImageTiling : unsigned int {
        OPTIMAL = VK_IMAGE_TILING_OPTIMAL,  /*!< The texels are stored in an arrangement that is optimal for GPU operations. */
        LINEAR = VK_IMAGE_TILING_LINEAR     /*!< The texels are stored in a linear arrangement for ease of access in CPU operations. */
    };
}
