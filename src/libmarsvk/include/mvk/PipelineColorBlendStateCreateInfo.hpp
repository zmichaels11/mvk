#pragma once

#include <vector>

#include "mvk/Color.hpp"
#include "mvk/LogicOp.hpp"
#include "mvk/PipelineColorBlendAttachmentState.hpp"

namespace mvk {
    struct PipelineColorBlendStateCreateInfo {
        unsigned int flags;
        bool logicOpEnable;
        LogicOp logicOp;
        std::vector<PipelineColorBlendAttachmentState> attachments;
        Color blendConstants;
    };
}
