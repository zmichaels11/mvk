#pragma once

#include "mvk/Color.hpp"
#include "mvk/DepthStencil.hpp"

namespace mvk {
    union ClearValue {
        Color color;
        DepthStencil depthStencil;
    };
}
