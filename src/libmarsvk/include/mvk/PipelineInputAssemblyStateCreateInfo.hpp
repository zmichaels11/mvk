#pragma once

#include "mvk/PrimitiveTopology.hpp"

namespace mvk {
    struct PipelineInputAssemblyStateCreateInfo {
        unsigned int flags;
        PrimitiveTopology topology;
        bool primitiveRestartEnable;
    };
}
