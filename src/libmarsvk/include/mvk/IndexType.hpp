#pragma once

#include "volk.h"

namespace mvk {
    //! The size of an index inside an index Buffer.
    enum class IndexType : unsigned int {
        UINT16 = VK_INDEX_TYPE_UINT16,  /*!< Specifies that indices are 16bit unsigned ints. */
        UINT32 = VK_INDEX_TYPE_UINT32   /*!< Specifies that indices are 32bit unsigned ints. It is not guaranteed that the entire 32bit range is available. */
    };
}
