#pragma once

#include "volk.h"

namespace mvk {
    enum class PhysicalDeviceType : unsigned int {
        OTHER = VK_PHYSICAL_DEVICE_TYPE_OTHER,
        INTEGRATED_GPU = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
        DISCRETE_GPU = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
        VIRTUAL_GPU = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
        CPU = VK_PHYSICAL_DEVICE_TYPE_CPU
    };
}
