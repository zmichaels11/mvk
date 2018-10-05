#pragma once

#include "mvk/AccessFlag.hpp"
#include "mvk/DependencyFlag.hpp"
#include "mvk/PipelineStageFlag.hpp"

namespace mvk {
    struct SubpassDependency {
        int srcSubpass;
        int dstSubpass;
        PipelineStageFlag srcStageMask;
        PipelineStageFlag dstStageMask;
        AccessFlag srcAccessMask;
        AccessFlag dstAccessMask;
        DependencyFlag dependencyFlags;
    };

    constexpr int SUBPASS_EXTERNAL = static_cast<int> (VK_SUBPASS_EXTERNAL);
}
