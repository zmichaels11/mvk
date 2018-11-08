#pragma once

#include "volk.h"

namespace mvk {
    //! Framebuffer blending operations.
    /*!
        Once the source and destination blend factors have been selected, 
        they along with the source and destination components are passed to the blending operations. 
        RGB and alpha components can use different operations.

        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBlendOp.html">VkBlendOp</a>
    */
    enum class BlendOp : unsigned int {
        ADD = VK_BLEND_OP_ADD,
        SUBTRACT = VK_BLEND_OP_SUBTRACT,
        REVERSE_SUBTRACT = VK_BLEND_OP_REVERSE_SUBTRACT,
        MIN = VK_BLEND_OP_MIN,
        MAX = VK_BLEND_OP_MAX    
    };
}
