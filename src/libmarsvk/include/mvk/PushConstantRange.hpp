#pragma once

#include "mvk/ShaderStage.hpp"

namespace mvk {
    struct PushConstantRange {
        ShaderStage stages;
        int offset;
        unsigned int size;
    };

    inline bool operator== (const PushConstantRange& lhs, const PushConstantRange& rhs) {
        return lhs.stages == rhs.stages
                && lhs.offset == rhs.offset
                && lhs.size == rhs.size;
    }
}
