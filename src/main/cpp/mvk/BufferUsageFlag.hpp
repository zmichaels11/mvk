#pragma once

#include "volk.h"

namespace mvk {
    enum class BufferUsageFlag : unsigned int {
        TRANSFER_SRC = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        TRANSFER_DST = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        UNIFORM_TEXEL_BUFFER = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
        STORAGE_TEXEL_BUFFER = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
        UNIFORM_BUFFER = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        STORAGE_BUFFER = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        INDEX_BUFFER = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VERTEX_BUFFER = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        INDIRECT_BUFFER = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
    };

    constexpr BufferUsageFlag operator| (BufferUsageFlag lhs, BufferUsageFlag rhs) {
        return static_cast<BufferUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr BufferUsageFlag operator& (BufferUsageFlag lhs, BufferUsageFlag rhs) {
        return static_cast<BufferUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
