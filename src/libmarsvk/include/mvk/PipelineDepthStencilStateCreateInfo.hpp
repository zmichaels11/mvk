#pragma once

#include "mvk/CompareOp.hpp"
#include "mvk/StencilOpState.hpp"

namespace mvk {
    struct PipelineDepthStencilStateCreateInfo {
        unsigned int flags;
        bool depthTestEnable;
        bool depthWriteEnable;
        CompareOp depthCompareOp;
        bool depthBoundsTestEnable;
        bool stencilTestEnable;
        StencilOpState front;
        StencilOpState back;
        float minDepthBounds;
        float maxDepthBounds;
    };
}
