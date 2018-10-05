#pragma once
#include "volk.h"

namespace mvk {
    enum class AccessFlag : unsigned int {
        NONE = 0,
        INDIRECT_COMMAND_READ = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
        INDEX_READ = VK_ACCESS_INDEX_READ_BIT,
        VERTEX_ATTRIBUTE_READ = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        UNIFORM_READ = VK_ACCESS_UNIFORM_READ_BIT,
        INPUT_ATTACHMENT_READ = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
        SHADER_READ = VK_ACCESS_SHADER_READ_BIT,
        SHADER_WRITE = VK_ACCESS_SHADER_WRITE_BIT,
        COLOR_ATTACHMENT_READ = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        COLOR_ATTACHMENT_WRITE = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        DEPTH_STENCIL_ATTACHMENT_READ = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
        DEPTH_STENCIL_ATTACHMENT_WRITE = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        TRANSFER_READ = VK_ACCESS_TRANSFER_READ_BIT,
        TRANSFER_WRITE = VK_ACCESS_TRANSFER_WRITE_BIT,
        HOST_READ = VK_ACCESS_HOST_READ_BIT,
        HOST_WRITE = VK_ACCESS_HOST_WRITE_BIT,
        MEMORY_READ = VK_ACCESS_MEMORY_READ_BIT,
        MEMORY_WRITE = VK_ACCESS_MEMORY_WRITE_BIT
    };

    constexpr AccessFlag operator| (AccessFlag lhs, AccessFlag rhs) {
        return static_cast<AccessFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr AccessFlag operator& (AccessFlag lhs, AccessFlag rhs) {
        return static_cast<AccessFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
