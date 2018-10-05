#pragma once

#include "volk.h"

namespace mvk {
    enum class PipelineBindPoint : unsigned int {
        GRAPHICS = VK_PIPELINE_BIND_POINT_GRAPHICS,
        COMPUTE = VK_PIPELINE_BIND_POINT_COMPUTE
    };
}
