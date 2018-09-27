#pragma once

#include "volk.h"

namespace mvk {
    enum class CommandPoolCreateFlag : unsigned int {
        CREATE_PROTECTED = VK_COMMAND_POOL_CREATE_PROTECTED_BIT,
        CREATE_RESET_COMMAND_BUFFER = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        CREATE_TRANSIENT = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
    };

    constexpr CommandPoolCreateFlag operator| (CommandPoolCreateFlag lhs, CommandPoolCreateFlag rhs) {
        return static_cast<CommandPoolCreateFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr CommandPoolCreateFlag operator& (CommandPoolCreateFlag lhs, CommandPoolCreateFlag rhs) {
        return static_cast<CommandPoolCreateFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
