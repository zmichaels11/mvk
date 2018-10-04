#pragma once

#include "mvk/AccessFlag.hpp"

namespace mvk {
    struct MemoryBarrier {
        AccessFlag srcAccessMask;
        AccessFlag dstAccessMask;
    };
}
