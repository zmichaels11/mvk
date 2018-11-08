#pragma once

#include "mvk/Color.hpp"
#include "mvk/DepthStencil.hpp"

namespace mvk {
    //! Structure specifying a clear value.
    /*!
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkClearValue.html">VkClearValue</a>
    */
    union ClearValue {
        Color color;                /*!< Specifies the color Image clear values to use when clearing a color Image or attachment. */
        DepthStencil depthStencil;  /*!< Specifies the depth and stencil clear values to use when clearing a depth/stencil Image or attachment. */
    };
}
