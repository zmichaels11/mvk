#pragma once

#include "volk.h"

namespace mvk {
    enum SamplerAddressMode : unsigned int {
        CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        REPEAT = VK_SAMPLER_ADDRESS_MODE_REPEAT
    };
}