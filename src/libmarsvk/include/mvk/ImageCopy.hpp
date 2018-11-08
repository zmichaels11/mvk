#pragma once

#include "mvk/Extent3D.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/Offset3D.hpp"

namespace mvk {
    //! ImageCopy defines an image copy operation.
    struct ImageCopy {
        Offset3D srcOffset;                     /*!< The offset into the source Image in texels to begin reading from. */
        Offset3D dstOffset;                     /*!< The offset into the destination Image in texels to begin writing to. */
        Extent3D extent;                        /*!< The size of the copy operation in texels. */
        ImageSubresourceLayers srcSubresource;  /*!< The subresource section from the source Image to copy from. */
        ImageSubresourceLayers dstSubresource;  /*!< The subresource section of the destination Image to copy to. */
    };
}
