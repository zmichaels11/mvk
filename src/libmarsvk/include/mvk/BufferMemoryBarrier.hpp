#pragma once

#include <cstddef>

#include "mvk/AccessFlag.hpp"

namespace mvk {
    class Buffer;
    class QueueFamily;

    //! Structure specifying a Buffer memory barrier.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBufferMemoryBarrier.html">VkBufferMemoryBarrier</a>
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-access-masks">Source Access Mask</a>
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-access-masks">Destination AccessMask</a>
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-queue-transfers">Queue Family Ownership Transfer</a>
    */
    struct BufferMemoryBarrier {
        AccessFlag srcAccessMask;               /*!< Bitmask of AccessFlag types specifying a source access mask. */
        AccessFlag dstAccessMask;               /*!< Bitmask of AccessFlag types specifying a destination access mask. */
        const QueueFamily * srcQueueFamily;     /*!< The source QueueFamily for a QueueFamily ownership transfer. */
        const QueueFamily * dstQueueFamily;     /*!< The destination QueueFamily for a QueueFamily ownership transfer. */
        const Buffer * buffer;                  /*!< The Buffer whose backing memory is affected by the barrier. */
        std::ptrdiff_t offset;                  /*!< The offset in bytes into the backing memory for the barrier. */
        std::size_t size;                       /*!< The size in bytes of the affected area of backing memory for Buffer. Can be WHOLE_SIZE */
    };
}
