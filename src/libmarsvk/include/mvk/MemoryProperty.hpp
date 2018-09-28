#pragma once

#include "volk.h"

namespace mvk {
    enum class MemoryProperty : unsigned int {
        DEVICE_LOCAL = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        HOST_VISIBLE = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        HOST_COHERENT = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        HOST_CACHED = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
        LAZILY_ALLOCATED = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
    };

    constexpr MemoryProperty operator| (MemoryProperty lhs, MemoryProperty rhs) {
        return static_cast<MemoryProperty> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr MemoryProperty operator& (MemoryProperty lhs, MemoryProperty rhs) {
        return static_cast<MemoryProperty> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
