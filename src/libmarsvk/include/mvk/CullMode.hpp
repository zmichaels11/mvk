#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask controlling triangle culling.
    enum class CullMode : unsigned int {
        FRONT = VK_CULL_MODE_FRONT_BIT,                 /*!< Specifies that front-facing triangles are discarded */
        BACK = VK_CULL_MODE_BACK_BIT,                   /*!< Specifies that back-facing triangles are discarded */
        FRONT_AND_BACK = VK_CULL_MODE_FRONT_AND_BACK,   /*!< Specifies that all triangles are discarded */
        NONE = VK_CULL_MODE_NONE                        /*!< Specifies that no triangles are discarded */
    };
}
