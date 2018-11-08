#pragma once

#include <cstddef>

namespace mvk {
    //! Structure specifying a buffer copy operation.
    /*
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBufferCopy.html">VkBufferCopy</a>
    */
    struct Buffercopy {
        std::ptrdiff_t srcOffset;   /*!< The starting offset in bytes from the start of srcBuffer. */
        std::ptrdiff_t dstOffset;   /*!< The starting offset in bytes from the start of dstBuffer. */
        std::size_t size;           /*!< The number of bytes to copy. */
    };
}
