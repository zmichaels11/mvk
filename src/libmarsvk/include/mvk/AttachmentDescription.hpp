#pragma once

#include "mvk/Format.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/LoadOp.hpp"
#include "mvk/StoreOp.hpp"

namespace mvk {
    //! Structure specifying an attachment description
    /*!
        <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkAttachmentDescription.html">VkAttachmentDescription</a>
    */
    struct AttachmentDescription {
        unsigned int flags;         /*!< Bitmask specifying additional properties of the attachment */
        Format format;              /*!< The format of the ImageView that will be used for the attachment */
        LoadOp loadOp;              /*!< Specifies how the contents of color and depth components are treated at the beginning of the RenderPass */
        StoreOp storeOp;            /*!< Specifies how the contents of the color and depth components are treated at the end of the RenderPass */
        LoadOp stencilLoadOp;       /*!< Specifies how the contents of the stencil components are treated at the beginning of the RenderPass */
        StoreOp stencilStoreOp;     /*!< Specifies how the contents of the stencil components are treated at the end of the RenderPass */
        ImageLayout initialLayout;  /*!< The layout the attachment Image subresource will be in at the beginning of the RenderPass */
        ImageLayout finalLayout;    /*!< The layout of the Image subresource will be transitioned to at the end of the RenderPass. */
        int samples;                /*!< The number of samples of the image */
    };
}
