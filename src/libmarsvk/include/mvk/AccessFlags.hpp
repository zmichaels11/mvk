#pragma once

#include "mvk/AccessFlag.hpp"

namespace mvk {
    namespace AccessFlags {
        constexpr auto INDIRECT_COMMAND_READ_ONLY = AccessFlag::INDIRECT_COMMAND_READ;
        constexpr auto INDEX_READ_ONLY = AccessFlag::INDEX_READ;
        constexpr auto VERTEX_ATTRIBUTE_READ_ONLY = AccessFlag::VERTEX_ATTRIBUTE_READ;
        constexpr auto UNIFORM_READ_ONLY = AccessFlag::UNIFORM_READ;
        constexpr auto INPUT_ATTACHMENT_READ_ONLY = AccessFlag::INPUT_ATTACHMENT_READ;
        constexpr auto SHADER_READ_ONLY = AccessFlag::SHADER_READ;
        constexpr auto SHADER_WRITE_ONLY = AccessFlag::SHADER_WRITE;
        constexpr auto SHADER_READ_WRITE = AccessFlag::SHADER_READ | AccessFlag::SHADER_WRITE;
        constexpr auto COLOR_ATTACHMENT_READ_ONLY = AccessFlag::COLOR_ATTACHMENT_READ;
        constexpr auto COLOR_ATTACHMENT_WRITE_ONLY = AccessFlag::COLOR_ATTACHMENT_WRITE;
        constexpr auto COLOR_ATTACHMENT_READ_WRITE = AccessFlag::COLOR_ATTACHMENT_READ | AccessFlag::COLOR_ATTACHMENT_WRITE;
        constexpr auto DEPTH_STENCIL_ATTACHMENT_READ_ONLY = AccessFlag::DEPTH_STENCIL_ATTACHMENT_READ;
        constexpr auto DEPTH_STENCIL_ATTACHMENT_WRITE_ONLY = AccessFlag::DEPTH_STENCIL_ATTACHMENT_WRITE;
        constexpr auto DEPTH_STENCIL_ATTACHMENT_READ_WRITE = AccessFlag::DEPTH_STENCIL_ATTACHMENT_READ | AccessFlag::DEPTH_STENCIL_ATTACHMENT_WRITE;
        constexpr auto TRANSFER_READ_ONLY = AccessFlag::TRANSFER_READ;
        constexpr auto TRANSFER_WRITE_ONLY = AccessFlag::TRANSFER_WRITE;
        constexpr auto TRANSFER_READ_WRITE = AccessFlag::TRANSFER_READ | AccessFlag::TRANSFER_WRITE;
        constexpr auto HOST_READ_ONLY = AccessFlag::HOST_READ;
        constexpr auto HOST_WRITE_ONLY = AccessFlag::HOST_WRITE;
        constexpr auto HOST_READ_WRITE = AccessFlag::HOST_READ | AccessFlag::HOST_WRITE;
        constexpr auto MEMORY_READ_ONLY = AccessFlag::MEMORY_READ;
        constexpr auto MEMORY_WRITE_ONLY = AccessFlag::MEMORY_WRITE;
        constexpr auto MEMORY_READ_WRITE = AccessFlag::MEMORY_READ | AccessFlag::MEMORY_WRITE;
        constexpr auto NONE = static_cast<AccessFlag> (0);
    }
}
