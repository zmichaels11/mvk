#pragma once

#include "mvk/AspectFlag.hpp"

namespace mvk {
    struct ImageSubresourceLayers {
        AspectFlag aspectMask;
        int mipLevel;
        int baseArrayLayer;
        int layerCount;
    };
}
