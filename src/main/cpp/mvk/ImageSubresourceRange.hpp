#pragma once

#include "mvk/AspectFlag.hpp"

namespace mvk {
    struct ImageSubresourceRange {
        AspectFlag aspectMask;
        int baseMipLevel;
        int levelCount;
        int baseArrayLayer;
        int layerCount;
    };
}
