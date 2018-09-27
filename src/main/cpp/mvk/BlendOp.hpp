#pragma once

#include "volk.h"

namespace mvk {
    enum class BlendOp : unsigned int {
        ADD = VK_BLEND_OP_ADD,
        SUBTRACT = VK_BLEND_OP_SUBTRACT,
        REVERSE_SUBTRACT = VK_BLEND_OP_REVERSE_SUBTRACT,
        MIN = VK_BLEND_OP_MIN,
        MAX = VK_BLEND_OP_MAX    
    };
}
