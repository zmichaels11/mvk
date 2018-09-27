#pragma once

#include "volk.h"

namespace mvk {
    namespace Util {
        constexpr VkDeviceSize alignUp(VkDeviceSize a, VkDeviceSize b) {
            return (a + b - 1) / b * b;
        }

        constexpr VkDeviceSize alignDown(VkDeviceSize a, VkDeviceSize b) {
            return a & (~b - 1);
        }
    }
}
