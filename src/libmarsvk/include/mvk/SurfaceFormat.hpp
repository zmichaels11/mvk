#pragma once

#include "mvk/ColorSpace.hpp"
#include "mvk/Format.hpp"

namespace mvk {
    struct SurfaceFormat {
        Format format;
        ColorSpace colorSpace;
    };
}
