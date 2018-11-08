#pragma once

#include "volk.h"

namespace mvk {
    enum class PresentMode : unsigned int {
        IMMEDIATE = VK_PRESENT_MODE_IMMEDIATE_KHR,
        FIFO = VK_PRESENT_MODE_FIFO_KHR,
        FIFO_RELAXED = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
        MAILBOX = VK_PRESENT_MODE_MAILBOX_KHR
    };
}
