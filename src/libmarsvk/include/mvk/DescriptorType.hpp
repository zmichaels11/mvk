#pragma once

#include "volk.h"

namespace mvk {
    //! DescriptorType is the type accepted by a DescriptorSet and describes a shader resource binding.
    enum class DescriptorType : unsigned int {
        SAMPLER = VK_DESCRIPTOR_TYPE_SAMPLER,                                   /*!< Specifies a Sampler descriptor. */
        COMBINED_IMAGE_SAMPLER = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,     /*!< Specifies a combined Image Sampler descriptor. */
        SAMPLED_IMAGE = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,                       /*!< Specifies a sampled Image descriptor. */
        STORAGE_IMAGE = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,                       /*!< Specifies a storage Image descriptor. */
        UNIFORM_TEXEL_BUFFER = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,         /*!< Specifies a uniform texel Buffer descriptor. */
        STORAGE_TEXEL_BUFFER = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,         /*!< Specifies a storage texel Buffer descriptor. */
        UNIFORM_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,     /*!< Specifies a uniform Buffer descriptor that supports dynamic offsets. */
        STORAGE_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,     /*!< Specifies a storage Buffer descriptor that supports dynamic offsets. */
        UNIFORM_BUFFER = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                     /*!< Specifies a uniform Buffer descriptor. */
        STORAGE_BUFFER = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                     /*!< Specifies a storage Buffer descriptor. */
        INPUT_ATTACHMENT = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT                  /*!< Specifies an input attachment descriptor. */
    };
}
