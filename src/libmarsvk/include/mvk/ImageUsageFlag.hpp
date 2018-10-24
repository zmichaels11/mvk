#pragma once

#include "volk.h"

namespace mvk {
    //! Bitmask that specifies usage limitations of an Image.
    enum class ImageUsageFlag : unsigned int {
        COLOR_ATTACHMENT = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,                     /*!< The Image can be used as a Framebuffer color attachment. */
        SAMPLED = VK_IMAGE_USAGE_SAMPLED_BIT,                                       /*!< The Image can be sampled in a shader. */
        DEPTH_STENCIL_ATTACHMENT = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,     /*!< The Image can be used as a Framebuffer depth-stencil attachment. */
        TRANSFER_DST = VK_IMAGE_USAGE_TRANSFER_DST_BIT,                             /*!< The Image can be used as the destination of a transfer operation. */
        TRANSFER_SRC = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,                             /*!< The Image can be used as the source of a transfer operation. */
        STORAGE = VK_IMAGE_USAGE_STORAGE_BIT,                                       /*!< The Image can be used as a shader storage Image. */
        INPUT_ATTACHMENT = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,                     /*!< The Image can be used as an input attachment. */
        TRANSIENT_ATTACHMENT = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT              /*!< The Image can be used as a transient attachment. */
    };

    inline constexpr ImageUsageFlag operator| (ImageUsageFlag lhs, ImageUsageFlag rhs) noexcept {
        return static_cast<ImageUsageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr ImageUsageFlag operator& (ImageUsageFlag lhs, ImageUsageFlag rhs) noexcept {
        return static_cast<ImageUsageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
