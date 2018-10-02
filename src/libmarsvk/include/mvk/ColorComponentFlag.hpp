#pragma once

#include "volk.h"

namespace mvk {
    enum class ColorComponentFlag : unsigned int {
        R = VK_COLOR_COMPONENT_R_BIT,
        G = VK_COLOR_COMPONENT_G_BIT,
        B = VK_COLOR_COMPONENT_B_BIT,
        A = VK_COLOR_COMPONENT_A_BIT
    };

    constexpr ColorComponentFlag operator& (const ColorComponentFlag& lhs, const ColorComponentFlag& rhs) {
        return static_cast<ColorComponentFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }

    constexpr ColorComponentFlag operator| (const ColorComponentFlag& lhs, const ColorComponentFlag& rhs) {
        return static_cast<ColorComponentFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }
}
