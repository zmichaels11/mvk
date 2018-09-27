#pragma once

#include "volk.h"

namespace mvk {
    enum class PresentMode : unsigned int {
        IMMEDIATE = VK_PRESENT_MODE_IMMEDIATE_KHR,
        VSYNC = VK_PRESENT_MODE_FIFO_KHR,
        ADAPTIVE_VSYNC = VK_PRESENT_MODE_FIFO_RELAXED_KHR
    };
}
