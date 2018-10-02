#pragma once

#include <vector>

#include "mvk/VertexInputAttributeDescription.hpp"
#include "mvk/VertexInputBindingDescription.hpp"

namespace mvk {
    struct PipelineVertexInputStateCreateInfo {
        unsigned int flags;
        std::vector<VertexInputBindingDescription> vertexBindingDescriptions;
        std::vector<VertexInputAttributeDescription> vertexAttributeDescriptions;
    };
}
