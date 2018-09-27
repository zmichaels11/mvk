#pragma once

#include "mvk/MemoryProperty.hpp"

namespace mvk {
    namespace MemoryProperties {
        constexpr auto DEVICE = MemoryProperty::DEVICE_LOCAL;
        constexpr auto SYSTEM = MemoryProperty::HOST_VISIBLE | MemoryProperty::HOST_COHERENT;
        constexpr auto SYSTEM_CACHED = MemoryProperty::HOST_VISIBLE | MemoryProperty::HOST_COHERENT | MemoryProperty::HOST_COHERENT;
        constexpr auto UNIFIED = MemoryProperty::DEVICE_LOCAL | MemoryProperty::HOST_VISIBLE | MemoryProperty::HOST_COHERENT;
        constexpr auto AMD_PINNED = UNIFIED;
    }
}
