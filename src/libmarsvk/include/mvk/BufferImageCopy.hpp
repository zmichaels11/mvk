#pragma once

#include <cstddef>

#include "mvk/Extent3D.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/Offset3D.hpp"

namespace mvk {
    //! Structure specifying a buffer image copy operation.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBufferImageCopy.html">VkBufferImageCopy</a>
    */
    struct BufferImageCopy {
        std::ptrdiff_t bufferOffset;                /*!< The offset in bytes from the start of the Buffer object. */
        int bufferImageHeight;                      /*!< The size of the image; used in 3D images. A value of 0 indicates tightly packed. */
        int bufferRowLength;                        /*!< The size of the image; used in 2D and 3D images. A value of 0 indicates tightly packed. */
        ImageSubresourceLayers imageSubresources;   /*!< The ImageSubresourceLayers for the source or destination image data. */
        Offset3D imageOffset;                       /*!< The initial <x, y, z> offsets in texels of the sub-region of the image data. */
        Extent3D imageExtent;                       /*!< The size in texels of the image to copy in <width, height, depth> */
    };
}
