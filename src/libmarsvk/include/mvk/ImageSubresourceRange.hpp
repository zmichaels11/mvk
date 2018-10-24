#pragma once

#include "mvk/AspectFlag.hpp"

namespace mvk {
    //! A range of subresources covering both level of detail and Image array layer.
    struct ImageSubresourceRange {
        AspectFlag aspectMask;  /*!< The AspectFlag bitmask */
        int baseMipLevel;       /*!< The highest level of detail. */
        int levelCount;         /*!< The number of mip levels. */
        int baseArrayLayer;     /*!< The lowest array layer. */
        int layerCount;         /*!< The number of array layers. */
    };
}
