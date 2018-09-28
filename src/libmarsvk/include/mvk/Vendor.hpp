#pragma once

#include "volk.h"

namespace mvk {
    enum class Vendor : unsigned int {
        AMD = 0x1002,
        NVIDIA = 0x10DE,
        INTEL = 0x8086,
        IMGTEX = 0x1010,
        ARM = 0x13B5,
        QUALCOMM = 0x5143
    };
}
