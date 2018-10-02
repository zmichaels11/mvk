#pragma once

#include "volk.h"

namespace mvk {
    enum class DependencyFlag : unsigned int {
        NONE = 0,
        BY_REGION = VK_DEPENDENCY_BY_REGION_BIT,
        DEVICE_GROUP = VK_DEPENDENCY_DEVICE_GROUP_BIT,
        VIEW_LOCAL = VK_DEPENDENCY_VIEW_LOCAL_BIT
    };

    constexpr DependencyFlag operator| (DependencyFlag lhs, DependencyFlag rhs) {
        return static_cast<DependencyFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr DependencyFlag operator& (DependencyFlag lhs, DependencyFlag rhs) {
        return static_cast<DependencyFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}