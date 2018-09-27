#pragma once

#include "volk.h"

namespace mvk {
    enum class VertexInputRate : unsigned int {
        PER_VERTEX = VK_VERTEX_INPUT_RATE_VERTEX,
        PER_INSTANCE = VK_VERTEX_INPUT_RATE_INSTANCE
    };
}
