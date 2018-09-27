#pragma once

#include "volk.h"

namespace mvk {
    enum class CommandBufferUsageFlag : unsigned int {
        ONE_TIME_SUBMIT = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
        SIMULTANEOUS_USE = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    constexpr CommandBufferUsageFlag operator| (CommandBufferUsageFlag lhs, CommandBufferUsageFlag rhs) {
        return static_cast<CommandBufferUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr CommandBufferUsageFlag operator& (CommandBufferUsageFlag lhs, CommandBufferUsageFlag rhs) {
        return static_cast<CommandBufferUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
