#pragma once

namespace mvk {
    //! Clear values for a Depth/Stencil component.
    struct DepthStencil {
        float depth;            /*!< The value to clear the depth component to. Usually should be 1.0; setting it to other values may have a negative performance impact. */
        unsigned int stencil;   /*!< The value to clear the stencil component to. Usually should be 0u; Setting it to other values may have a negative performance impact. */
    };
}
