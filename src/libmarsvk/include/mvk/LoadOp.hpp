#pragma once

#include "volk.h"

namespace mvk {
    //! Operation controls for attachment load behavior.
    enum class LoadOp : unsigned int {
        DONT_CARE = VK_ATTACHMENT_LOAD_OP_DONT_CARE,    /*!< The drivers are allowed to choose how to load an attachment. This is probably the fastest if the entire attachment is written during the RenderPass. */
        LOAD = VK_ATTACHMENT_LOAD_OP_LOAD,              /*!< Restores the attachment from the prior RenderPass prior to executing the current RenderPass. This is probably the slowest attachment load operation. */
        CLEAR = VK_ATTACHMENT_LOAD_OP_CLEAR             /*!< Clears the attachment to the specified ClearValues before writing the RenderPass. Depending on the chosen ClearValues, this operation may be fast. Fast ClearValues are vendor-specific. */
    };
}
