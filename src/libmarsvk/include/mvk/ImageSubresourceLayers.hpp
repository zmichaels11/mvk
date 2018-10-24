#pragma once

#include "mvk/AspectFlag.hpp"

namespace mvk {
    //! A range of Image layers that all belong to the same level of detail.
    struct ImageSubresourceLayers {
        AspectFlag aspectMask;  /*!< The AspectFlag bitmask of the subresource layers */
        int mipLevel;           /*!< The level of detail. */
        int baseArrayLayer;     /*!< The first array layer. */
        int layerCount;         /*!< The number of array layers. */
    };
}
