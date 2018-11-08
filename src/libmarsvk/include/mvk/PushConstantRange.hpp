#pragma once

#include "mvk/ShaderStage.hpp"

namespace mvk {
    struct PushConstantRange {
        ShaderStage stages;
        int offset;
        unsigned int size;
    };

    inline constexpr bool operator== (const PushConstantRange& lhs, const PushConstantRange& rhs) noexcept {
        return lhs.stages == rhs.stages
                && lhs.offset == rhs.offset
                && lhs.size == rhs.size;
    }
}
