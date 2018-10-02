#pragma once

#include "mvk/CompareOp.hpp"
#include "mvk/StencilOp.hpp"

namespace mvk {
    struct StencilOpState {
        StencilOp failOp;
        StencilOp passOp;
        StencilOp depthFailOp;
        CompareOp compareOp;
        unsigned int compareMask;
        unsigned int writeMask;
        unsigned int reference;
    };
}
