#pragma once

#include "volk.h"

namespace mvk {
    enum class SubpassContents : unsigned int {
        INLINE = VK_SUBPASS_CONTENTS_INLINE,
        SECONDARY_COMMAND_BUFFERS = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
    };
}
