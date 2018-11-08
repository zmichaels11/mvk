#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask specifying how execution and memory dependencies are formed.
    enum class DependencyFlag : unsigned int {
        NONE = 0,                                       /*!< No memory or execution dependencies. */
        BY_REGION = VK_DEPENDENCY_BY_REGION_BIT,        /*!< Specifies that dependencies will be framebuffer-local. */
        DEVICE_GROUP = VK_DEPENDENCY_DEVICE_GROUP_BIT,  /*!< Specifies that dependencies are non-device-local dependency.*/
        VIEW_LOCAL = VK_DEPENDENCY_VIEW_LOCAL_BIT       /*!< Specifies that a subpass has more than one view. */
    };

    inline constexpr DependencyFlag operator| (DependencyFlag lhs, DependencyFlag rhs) noexcept {
        return static_cast<DependencyFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr DependencyFlag operator& (DependencyFlag lhs, DependencyFlag rhs) noexcept {
        return static_cast<DependencyFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
