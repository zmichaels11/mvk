#pragma once

#include "volk.h"

namespace mvk {
    //! The dimensionality and array state of an ImageView.
    enum class ImageViewType : unsigned int {
        VIEW_1D = VK_IMAGE_VIEW_TYPE_1D,                    /*!< Specifies that the ImageView has 1 dimension. */
        VIEW_2D = VK_IMAGE_VIEW_TYPE_2D,                    /*!< Specifies that the ImageView has 2 dimensions. */
        VIEW_3D = VK_IMAGE_VIEW_TYPE_3D,                    /*!< Specifies that the ImageView has 3 dimensions. */
        VIEW_1D_ARRAY = VK_IMAGE_VIEW_TYPE_1D_ARRAY,        /*!< Specifies that the ImageView has 1 dimension and is arrayed. */
        VIEW_2D_ARRAY = VK_IMAGE_VIEW_TYPE_2D_ARRAY,        /*!< Specifies that the ImageView has 2 dimensions and is arrayed. */
        VIEW_CUBE = VK_IMAGE_VIEW_TYPE_CUBE,                /*!< Specifies that the ImageView is for cube mapping. */
        VIEW_CUBE_ARRAY = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY     /*!< Specifies that the ImageView is for cube mapping and is arrayed. */
    };
}
