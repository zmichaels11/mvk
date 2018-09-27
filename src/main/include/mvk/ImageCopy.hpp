#pragma once

#include "mvk/Extent3D.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/Offset3D.hpp"

namespace mvk {
    struct ImageCopy {
        Offset3D srcOffset;
        Offset3D dstOffset;
        Extent3D extent;
        ImageSubresourceLayers srcSubresource;
        ImageSubresourceLayers dstSubresource;
    };
}
