#pragma once

#include "volk.h"

namespace mvk {
    //! The level of a CommandBuffer.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCommandBufferLevel.html">VkCommandBufferLevel</a>
    */
    enum class CommandBufferLevel : unsigned int {
        PRIMARY = VK_COMMAND_BUFFER_LEVEL_PRIMARY,      /*!< Specifies a primary CommandBuffer. */
        SECONDARY = VK_COMMAND_BUFFER_LEVEL_SECONDARY   /*!< Specifies a secondary CommandBuffer. These can be recorded into other CommandBuffers. */
    };
}
