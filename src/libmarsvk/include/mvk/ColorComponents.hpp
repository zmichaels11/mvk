#pragma once

#include "mvk/ColorComponentFlag.hpp"

namespace mvk {
    namespace ColorComponents {
        //! Bitmask of ColorComponentFlag that specify all channels.
        inline constexpr ColorComponentFlag rgba() noexcept {
            return ColorComponentFlag::R | ColorComponentFlag::G | ColorComponentFlag::B | ColorComponentFlag::A;
        }

        //! Bitmask of ColorComponentFlag that specify all channels except A.
        inline constexpr ColorComponentFlag rgb() noexcept {
            return ColorComponentFlag::R | ColorComponentFlag::G | ColorComponentFlag::B;
        }

        //! Bitmask of ColorComponentFlag that specify only the R and G channels.
        inline constexpr ColorComponentFlag rg() noexcept {
            return ColorComponentFlag::R | ColorComponentFlag::G;
        }
    }
}
