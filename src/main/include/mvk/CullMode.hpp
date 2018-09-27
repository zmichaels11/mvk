#pragma once

#include "volk.h"

namespace mvk {
    enum class CullMode : unsigned int {
        FRONT = VK_CULL_MODE_FRONT_BIT,
        BACK = VK_CULL_MODE_BACK_BIT,
        FRONT_AND_BACK = VK_CULL_MODE_FRONT_AND_BACK,
        NONE = VK_CULL_MODE_NONE
    };
}
