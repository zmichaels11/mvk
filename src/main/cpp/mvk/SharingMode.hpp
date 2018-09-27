#pragma once

#include "volk.h"

namespace mvk {
    enum class SharingMode : unsigned int {
        EXCLUSIVE = VK_SHARING_MODE_EXCLUSIVE,
        CONCURRENT = VK_SHARING_MODE_CONCURRENT
    };
}
