#pragma once

#include "mvk/ImageLayout.hpp"

namespace mvk {
    //! Structure specifying an attachment reference.
    /*!
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkAttachmentReference.html">VkAttachmentReference</a>
    */
    struct AttachmentReference {
        int attachment;         /*!< The index of the attachment of the RenderPass. */
        ImageLayout layout;     /*!< The layout of the attachment. */
    };
}
