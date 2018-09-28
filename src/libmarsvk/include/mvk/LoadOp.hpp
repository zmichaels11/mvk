#pragma once

#include "volk.h"

namespace mvk {
    enum class LoadOp : unsigned int {
        DONT_CARE = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        LOAD = VK_ATTACHMENT_LOAD_OP_LOAD,
        CLEAR = VK_ATTACHMENT_LOAD_OP_CLEAR
    };
}
