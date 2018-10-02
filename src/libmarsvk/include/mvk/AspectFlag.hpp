#pragma once

#include "volk.h"

namespace mvk {
    enum class AspectFlag : unsigned int {
        COLOR = VK_IMAGE_ASPECT_COLOR_BIT,
        DEPTH = VK_IMAGE_ASPECT_DEPTH_BIT,
        STENCIL = VK_IMAGE_ASPECT_STENCIL_BIT
    };

    inline constexpr AspectFlag operator| (const AspectFlag& lhs, const AspectFlag& rhs) {
        return static_cast<AspectFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr AspectFlag operator& (const AspectFlag& lhs, const AspectFlag& rhs) {
        return static_cast<AspectFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
