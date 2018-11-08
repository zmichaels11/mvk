#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask specifying allowed usage of a Buffer.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBufferUsageFlagBits.html">VkBufferUsageFlagBits</a>
    */
    enum class BufferUsageFlag : unsigned int {
        TRANSFER_SRC = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,                    /*!< Specifies that the Buffer can be used as the source of a transfer command. */
        TRANSFER_DST = VK_BUFFER_USAGE_TRANSFER_DST_BIT,                    /*!< Specifies that the Buffer can be used as the destination of a transfer command. */
        UNIFORM_TEXEL_BUFFER = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,    /*!< Specifies that the Buffer can be used to create a BufferView and be bound as a Uniform TexelBuffer. */
        STORAGE_TEXEL_BUFFER = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,    /*!< Specifies that the Buffer can be used to create a BufferView and be bound as a Storage TexelBuffer. */
        UNIFORM_BUFFER = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,                /*!< Specifies that the Buffer can be bound as a Uniform Buffer. */
        STORAGE_BUFFER = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,                /*!< Specifies that the Buffer can be bound as a Storage Buffer. */
        INDEX_BUFFER = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,                    /*!< Specifies that the Buffer can be bound as an Index Buffer for draw operations. */
        VERTEX_BUFFER = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,                  /*!< Specifies that the Buffer can be bound as a Vertex Buffer for draw operations. */
        INDIRECT_BUFFER = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT               /*!< Specifies that the Buffer can be bound as an Indirect Command Buffer for draw or compute operations. */
    };

    inline constexpr BufferUsageFlag operator| (BufferUsageFlag lhs, BufferUsageFlag rhs) noexcept {
        return static_cast<BufferUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr BufferUsageFlag operator& (BufferUsageFlag lhs, BufferUsageFlag rhs) noexcept {
        return static_cast<BufferUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
