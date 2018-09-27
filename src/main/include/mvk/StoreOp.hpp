#pragma once

#include "volk.h"

namespace mvk {
    enum class StoreOp : unsigned int {
        DONT_CARE = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        STORE = VK_ATTACHMENT_STORE_OP_STORE
    };
}
