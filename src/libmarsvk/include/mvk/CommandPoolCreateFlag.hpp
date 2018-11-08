#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask specifying usage behavior for a CommandPool.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCommandPoolCreateFlagBits.html">VkCommandPoolCreateFlagBits</a>
    */
    enum class CommandPoolCreateFlag : unsigned int {
        CREATE_PROTECTED = VK_COMMAND_POOL_CREATE_PROTECTED_BIT,                        /*!< Specifies that CommandBuffers allocated from the CommandPool are protected CommandBuffers. */
        CREATE_RESET_COMMAND_BUFFER = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,  /*!< Allows any CommandBuffer allocated from the CommandPool to be individually reset to the initial state. */
        CREATE_TRANSIENT = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT                         /*!< Specifies that CommandBuffers allocated from the CommandPool will be short-lived. */
    };

    inline constexpr CommandPoolCreateFlag operator| (CommandPoolCreateFlag lhs, CommandPoolCreateFlag rhs) noexcept {
        return static_cast<CommandPoolCreateFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr CommandPoolCreateFlag operator& (CommandPoolCreateFlag lhs, CommandPoolCreateFlag rhs) noexcept {
        return static_cast<CommandPoolCreateFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
