#pragma once

#include "volk.h"

namespace mvk {
    enum class CommandBufferLevel : unsigned int {
        PRIMARY = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        SECONDARY = VK_COMMAND_BUFFER_LEVEL_SECONDARY
    };
}
