#pragma once

#include "mvk/ShaderStage.hpp"

namespace mvk {
    struct PushConstantRange {
        ShaderStage stages;
        int offset;
        unsigned int size;
    };
}
