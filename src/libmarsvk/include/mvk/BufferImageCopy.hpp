#pragma once

#include <cstddef>

#include "mvk/Extent3D.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/Offset3D.hpp"

namespace mvk {
    struct BufferImageCopy {
        std::ptrdiff_t bufferOffset;
        int bufferImageHeight;
        int bufferRowLength;
        ImageSubresourceLayers imageSubresources;
        Offset3D imageOffset;
        Extent3D imageExtent;
    };
}
