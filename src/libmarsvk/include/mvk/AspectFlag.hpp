#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask specifying which aspects of an image are included in a view
    enum class AspectFlag : unsigned int {
        NONE = 0,
        COLOR = VK_IMAGE_ASPECT_COLOR_BIT,                                          /*!< Specifies the color aspect */
        DEPTH = VK_IMAGE_ASPECT_DEPTH_BIT,                                          /*!< Specifies the depth aspect */
        STENCIL = VK_IMAGE_ASPECT_STENCIL_BIT,                                      /*!< Specifies the stencil aspect */
        DEPTH_STENCIL = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT     /*!< Specifies a combined depth-stencil aspect */
    };

    inline constexpr AspectFlag operator| (const AspectFlag& lhs, const AspectFlag& rhs) noexcept {
        return static_cast<AspectFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr AspectFlag operator& (const AspectFlag& lhs, const AspectFlag& rhs) noexcept {
        return static_cast<AspectFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
