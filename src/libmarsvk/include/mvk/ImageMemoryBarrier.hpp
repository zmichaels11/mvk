#pragma once

#include "mvk/AccessFlag.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/ImageSubresourceRange.hpp"

namespace mvk {
    class Image;
    class QueueFamily;

    struct ImageMemoryBarrier {
        AccessFlag srcAccessMask;
        AccessFlag dstAccessMask;
        ImageLayout oldLayout;
        ImageLayout newLayout;
        const QueueFamily * srcQueueFamily;
        const QueueFamily * dstQueueFamily;
        const Image * image;
        ImageSubresourceRange subresourceRange;
    };
}
