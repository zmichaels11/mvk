#pragma once

#include "volk.h"

namespace mvk {
    enum class ImageUsageFlag : unsigned int {
        COLOR_ATTACHMENT = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        SAMPLED = VK_IMAGE_USAGE_SAMPLED_BIT,
        DEPTH_STENCIL_ATTACHMENT = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        TRANSFER_DST = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        TRANSFER_SRC = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        STORAGE = VK_IMAGE_USAGE_STORAGE_BIT,
        INPUT_ATTACHMENT = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        TRANSIENT_ATTACHMENT = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT
    };

    constexpr ImageUsageFlag operator| (ImageUsageFlag lhs, ImageUsageFlag rhs) {
        return static_cast<ImageUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr ImageUsageFlag operator& (ImageUsageFlag lhs, ImageUsageFlag rhs) {
        return static_cast<ImageUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
