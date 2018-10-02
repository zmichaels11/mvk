#pragma once

#include "mvk/CullMode.hpp"
#include "mvk/FrontFace.hpp"
#include "mvk/PolygonMode.hpp"

namespace mvk {
    struct PipelineRasterizationStateCreateInfo {
        unsigned int flags;
        bool depthClampEnable;
        bool rasterizationDiscardEnable;
        PolygonMode polygonMode;
        CullMode cullMode;
        FrontFace frontFace;
        bool depthBiasEnable;
        float depthBiasConstantFactor;
        float depthBiasClamp;
        float depthBiasSlopeFactor;
        float lineWidth;
    };
}
