#pragma once

#include "volk.h"

namespace mvk {
    //! Specify border color used for texture lookups.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBorderColor.html">VkBorderColor</a>
    */
    enum class BorderColor : unsigned int {
        FLOAT_OPAQUE_BLACK = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,            /*!< Specifies an opaque, floating-point format, black color */
        FLOAT_OPAQUE_WHITE = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,            /*!< Specifies an opaque, floating-point format, white color */
        FLOAT_TRANSPARENT_BLACK = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,  /*!< Specifies a transparent, floating-point format, black color */
        INT_OPAQUE_BLACK = VK_BORDER_COLOR_INT_OPAQUE_BLACK,                /*!< Specifies an opaque, integer format black color */
        INT_OPAQUE_WHITE = VK_BORDER_COLOR_INT_OPAQUE_WHITE,                /*!< Specifies an opaque, integer format white color */
        INT_TRANSPARENT_BLACK = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK       /*!< Specifies a transparent, integer format black color */
    };
}
