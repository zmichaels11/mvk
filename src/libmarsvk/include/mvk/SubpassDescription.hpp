#pragma once

#include <vector>

#include "mvk/PipelineBindpoint.hpp"
#include "mvk/AttachmentReference.hpp"

namespace mvk {
    struct SubpassDescription {
        unsigned int flags;
        PipelineBindpoint pipelineBindpoint;
        std::vector<AttachmentReference> inputAttachments;
        std::vector<AttachmentReference> colorAttachments;
        std::vector<AttachmentReference> resolveAttachments;
        std::vector<int> preserveAttachments;
        std::vector<AttachmentReference> depthStencilAttachment;
    };
}
