#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask specifying usage behavior for a CommandBuffer.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCommandBufferUsageFlagBits.html">VkCommandBufferUsageFlagBits</a>
    */
    enum class CommandBufferUsageFlag : unsigned int {
        ONE_TIME_SUBMIT = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,              /*!< Specifies that each recording of the CommandBuffer will only be submitted once and that the CommandBuffer will implicitly be reset after consumption. */
        RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,    /*!< Specifies that a Secondary CommandBuffer is considered to be entirely inside a RenderPass. */
        SIMULTANEOUS_USE = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT             /*!< Specifies that a CommandBuffer can be resubmitted to a Queue while it is in the Pending State. In addition, secondary CommandBuffers can be recorded into multiple Primary CommandBuffers. */
    };

    inline constexpr CommandBufferUsageFlag operator| (CommandBufferUsageFlag lhs, CommandBufferUsageFlag rhs) noexcept {
        return static_cast<CommandBufferUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr CommandBufferUsageFlag operator& (CommandBufferUsageFlag lhs, CommandBufferUsageFlag rhs) noexcept {
        return static_cast<CommandBufferUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
