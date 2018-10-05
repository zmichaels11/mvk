#pragma once

#include <vector>

#include "mvk/PipelineBindPoint.hpp"
#include "mvk/AttachmentReference.hpp"

namespace mvk {
    struct SubpassDescription {
        unsigned int flags;
        PipelineBindPoint pipelineBindPoint;
        std::vector<AttachmentReference> inputAttachments;
        std::vector<AttachmentReference> colorAttachments;
        std::vector<AttachmentReference> resolveAttachments;
        std::vector<int> preserveAttachments;
        std::vector<AttachmentReference> depthStencilAttachment;
    };
}
