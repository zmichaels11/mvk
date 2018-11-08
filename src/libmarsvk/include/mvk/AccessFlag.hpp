#pragma once
#include "volk.h"

namespace mvk {
    //! Memory Access Flags used for specifying memory dependencies.
    /*!
        Memory in Vulkan can be accessed from within shader invocations and 
        via some fixed-function stages of the pipeline. The access type is a 
        function of the descriptor type used, or how a fixed-function stage 
        accesses memory. Each access type corresponds to a bit flag in VkAccessFlagBits.

        Some synchronization commands take sets of access types as parameters to define 
        the access scopes of a memory dependency. If a synchronization command includes a 
        source access mask, its first access scope only includes accesses via the access types 
        specified in that mask. Similarly, if a synchronization command includes a destination 
        access mask, its second access scope only includes accesses via the access types 
        specified in that mask.
     */
    enum class AccessFlag : unsigned int {
        NONE = 0,
        INDIRECT_COMMAND_READ = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,                    /*!< Read access to indirect command data as part of an indirect drawing or dispatch command. */
        INDEX_READ = VK_ACCESS_INDEX_READ_BIT,                                          /*!< Read access to an index buffer as part of an indexed drawing command. */
        VERTEX_ATTRIBUTE_READ = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,                    /*!< Read access to a vertex buffer as a part of a drawing command. */
        UNIFORM_READ = VK_ACCESS_UNIFORM_READ_BIT,                                      /*!< Read access to a uniform buffer. */
        INPUT_ATTACHMENT_READ = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,                    /*!< Read access to an input attachment within a render pass during fragment shading. */
        SHADER_READ = VK_ACCESS_SHADER_READ_BIT,                                        /*!< Read access to a storage buffer, uniform texel buffer, storage texel buffer, sampled image, or storage image. */
        SHADER_WRITE = VK_ACCESS_SHADER_WRITE_BIT,                                      /*!< Write access to a storage buffer, storage texel buffer, or storage image. */
        COLOR_ATTACHMENT_READ = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,                    /*!< Read access to a color attachment */
        COLOR_ATTACHMENT_WRITE = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,                  /*!< Write access to a color or resolve attachment during a render pass. */
        DEPTH_STENCIL_ATTACHMENT_READ = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,    /*!< Read access to a depth/stencil attachment. */
        DEPTH_STENCIL_ATTACHMENT_WRITE = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,  /*!< Write access to a depth/stencil attachment. */
        TRANSFER_READ = VK_ACCESS_TRANSFER_READ_BIT,                                    /*!< Read access to an image or buffer in a copy operation. */
        TRANSFER_WRITE = VK_ACCESS_TRANSFER_WRITE_BIT,                                  /*!< Write access to an image or buffer in a copy operation.*/
        HOST_READ = VK_ACCESS_HOST_READ_BIT,                                            /*!< Read access by a host operation. */
        HOST_WRITE = VK_ACCESS_HOST_WRITE_BIT,                                          /*!< Write access by a host operation. */
        MEMORY_READ = VK_ACCESS_MEMORY_READ_BIT,                                        /*!< Read access to entities external of Vulkan. */
        MEMORY_WRITE = VK_ACCESS_MEMORY_WRITE_BIT                                       /*!< Write access to entities external to Vulkan. */
    };

    inline constexpr AccessFlag operator| (AccessFlag lhs, AccessFlag rhs) noexcept {
        return static_cast<AccessFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline constexpr AccessFlag operator& (AccessFlag lhs, AccessFlag rhs) noexcept {
        return static_cast<AccessFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
