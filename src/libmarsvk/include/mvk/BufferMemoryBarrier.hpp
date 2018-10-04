#pragma once

#include <cstddef>

#include "mvk/AccessFlag.hpp"

namespace mvk {
    class Buffer;
    class QueueFamily;

    struct BufferMemoryBarrier {
        AccessFlag srcAccessMask;
        AccessFlag dstAccessMask;
        const QueueFamily * srcQueueFamily;
        const QueueFamily * dstQueueFamily;
        const Buffer * buffer;
        std::ptrdiff_t offset;
        std::size_t size;
    };
}
