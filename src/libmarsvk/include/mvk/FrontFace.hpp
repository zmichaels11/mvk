#pragma once

#include "volk.h"

namespace mvk {
    //! Triangle vertex ordering for determining the front face of a triangle.
    enum class FrontFace : unsigned int {
        COUNTER_CLOCKWISE = VK_FRONT_FACE_COUNTER_CLOCKWISE,    /*!< Vertices iterate counter-clockwise to determine front-face. */
        CLOCKWISE = VK_FRONT_FACE_CLOCKWISE                     /*!< Vertices iterate clockwise to determine front-face. */
    };
}
