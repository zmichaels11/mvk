#pragma once

#include "volk.h"

namespace mvk {
    enum class IndexType : unsigned int {
        UINT16 = VK_INDEX_TYPE_UINT16,
        UINT32 = VK_INDEX_TYPE_UINT32
    };
}
