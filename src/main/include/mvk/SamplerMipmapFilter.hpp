#pragma once

#include "volk.h"

namespace mvk {
    enum class SamplerMipmapFilter : unsigned int {
        NEAREST = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        LINEAR = VK_SAMPLER_MIPMAP_MODE_LINEAR
    };
}
