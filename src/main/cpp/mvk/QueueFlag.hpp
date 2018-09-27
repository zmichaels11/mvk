#pragma once

#include "volk.h"

namespace mvk {
    enum class QueueFlag : unsigned int {
        GRAPHICS = VK_QUEUE_GRAPHICS_BIT,
        COMPUTE = VK_QUEUE_COMPUTE_BIT,
        TRANSFER = VK_QUEUE_TRANSFER_BIT,
        SPARSE_BINDING = VK_QUEUE_SPARSE_BINDING_BIT,
        PROTECTED = VK_QUEUE_PROTECTED_BIT
    };

    constexpr QueueFlag operator| (QueueFlag lhs, QueueFlag rhs) {
        return static_cast<QueueFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr QueueFlag operator& (QueueFlag lhs, QueueFlag rhs) {
        return static_cast<QueueFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
