#pragma once

#include "mvk/BlendFactor.hpp"
#include "mvk/BlendOp.hpp"
#include "mvk/ColorComponentFlag.hpp"

namespace mvk {
    struct PipelineColorBlendAttachmentState {
        bool blendEnable;
        BlendFactor srcColorBlendFactor;
        BlendFactor dstColorBlendFactor;
        BlendOp colorBlendOp;
        BlendFactor srcAlphaBlendFactor;
        BlendFactor dstAlphaBlendFactor;
        BlendOp alphaBlendOp;
        ColorComponentFlag colorWriteMask;
    };
}
