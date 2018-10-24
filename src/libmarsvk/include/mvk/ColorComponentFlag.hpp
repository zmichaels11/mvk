#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask controlling which components are written to the framebuffer
    /*!
        Bits which can be set in a PipelineColorBlendAttachmentState to determine whether the final color values are written to the framebuffer.
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkColorComponentFlagBits.html">VkColorComponentFlagBits</a>
    */
    enum class ColorComponentFlag : unsigned int {
        R = VK_COLOR_COMPONENT_R_BIT,   /*!< Specifies that the RED values are written to the color attachment. */
        G = VK_COLOR_COMPONENT_G_BIT,   /*!< Specifies that the GREEN values are written to the color attachment. */
        B = VK_COLOR_COMPONENT_B_BIT,   /*!< Specifies that the BLUE values are written to the color attachment. */
        A = VK_COLOR_COMPONENT_A_BIT    /*!< Specifies that the ALPHA values are written to the color attachment. */
    };

    inline constexpr ColorComponentFlag operator& (const ColorComponentFlag& lhs, const ColorComponentFlag& rhs) noexcept {
        return static_cast<ColorComponentFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }

    inline constexpr ColorComponentFlag operator| (const ColorComponentFlag& lhs, const ColorComponentFlag& rhs) noexcept {
        return static_cast<ColorComponentFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }
}
