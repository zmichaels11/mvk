#pragma once

#include "volk.h"

namespace mvk {
    //! Base level dimensionality of an Image.
    enum class ImageType : unsigned int {
        IMAGE_1D = VK_IMAGE_TYPE_1D,    /*!< The Image is 1 dimension. */
        IMAGE_2D = VK_IMAGE_TYPE_2D,    /*!< The Image is 2 dimensions. */
        IMAGE_3D = VK_IMAGE_TYPE_3D     /*!< The Image is 3 dimensions. */
    };
}
