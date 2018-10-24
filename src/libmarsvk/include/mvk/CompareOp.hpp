#pragma once

#include "volk.h"

namespace mvk {
    //! Stencil comparison functions.
    /*!
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCompareOp.html">VkCompareOp</a>
    */
    enum class CompareOp : unsigned int {
        NEVER = VK_COMPARE_OP_NEVER,                        /*!< Specifies that the test never passes. */
        LESS = VK_COMPARE_OP_LESS,                          /*!< Specifies that the test passes when R < S. */
        EQUAL = VK_COMPARE_OP_EQUAL,                        /*!< Specifies that the test passes when R = S. */
        LESS_OR_EQUAL = VK_COMPARE_OP_LESS_OR_EQUAL,        /*!< Specifies that the test passes when R ≤ S. */
        GREATER = VK_COMPARE_OP_GREATER,                    /*!< Specifies that the test passes when R > S. */
        NOT_EQUAL = VK_COMPARE_OP_NOT_EQUAL,                /*!< Specifies that the test passes when R ≠ S. */
        GREATER_OR_EQUAL = VK_COMPARE_OP_GREATER_OR_EQUAL,  /*!< Specifies that the test passes when R ≥ S. */
        ALWAYS = VK_COMPARE_OP_ALWAYS                       /*!< Specifies that the test always passes. */
    };
}
