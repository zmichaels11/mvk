#pragma once

#include "mvk/AccessFlag.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/ImageSubresourceRange.hpp"

namespace mvk {
    class Image;
    class QueueFamily;

    //! A structure specifying an Image memory barrier.
    struct ImageMemoryBarrier {
        AccessFlag srcAccessMask;                   /*!< The AccessFlag bitmask of the Image at the source stage. */
        AccessFlag dstAccessMask;                   /*!< The AccessFlag bitmask of the Image at the destination stage. */
        ImageLayout oldLayout;                      /*!< The ImageLayout of the Image at the source stage. */
        ImageLayout newLayout;                      /*!< The ImageLayout of the Image at the destination stage. */
        const QueueFamily * srcQueueFamily;         /*!< The QueueFamily of the Image at the source stage. Only used in a QueueFamily transfer operation. */
        const QueueFamily * dstQueueFamily;         /*!< The QueueFamily of the Image at the destination stage. Only used in a QueueFamily transfer operation. */
        const Image * image;                        /*!< The Image */
        ImageSubresourceRange subresourceRange;     /*!< The range of subresources affected by the ImageMemoryBarrier. */
    };
}
