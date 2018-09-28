#pragma once

#include "volk.h"

namespace mvk {
    enum class PipelineBindpoint : unsigned int {
        GRAPHICS = VK_PIPELINE_BIND_POINT_GRAPHICS,
        COMPUTE = VK_PIPELINE_BIND_POINT_COMPUTE
    };
}
