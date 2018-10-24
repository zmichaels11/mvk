#pragma once

#include "volk.h"

namespace mvk {
    //! Memory layouts for Image objects.
    enum class ImageLayout : unsigned int {
        COLOR_ATTACHMENT = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,                    /*!< Specifies an ImageLayout optimized for Framebuffer color attachments. */
        DEPTH_STENCIL_ATTACHMENT = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,    /*!< Specifies an ImageLayout optimized for Framebuffer depth-stencil attachments. */
        DEPTH_STENCIL_READ_ONLY = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,      /*!< Specifies an ImageLayout optimized for depth-stencil Image read-only operations. */
        PREINITIALIZED = VK_IMAGE_LAYOUT_PREINITIALIZED,                                /*!< Specifies an ImageLayout that is initialized on Image construction. */
        SHADER_READ_ONLY = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,                    /*!< Specifies an ImageLayout optimized for color Image read-only operations. */
        TRANSFER_SRC = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,                            /*!< Specifies an ImageLayout optimized for copy read operations. */
        TRANSFER_DST = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,                            /*!< Specifies an ImageLayout optimized for copy write operations. */
        UNDEFINED = VK_IMAGE_LAYOUT_UNDEFINED,                                          /*!< Specifies an ImageLayout that contains invalidated data. It is often more efficient to invalidate data than to clear it. */
        GENERAL = VK_IMAGE_LAYOUT_GENERAL,                                              /*!< Specifies an ImageLayout that can be used for any operation. This format is generally not as efficient as any other format. */
        PRESENT_SRC_KHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                               /*!< Specifies an ImageLayout optimized for presentation operations. */
    };
}
