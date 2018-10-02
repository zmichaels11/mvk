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
}
