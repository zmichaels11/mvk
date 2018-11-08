#pragma once

namespace mvk {
    //! A RGBA color structure.
    struct Color {
        float red;      /*!< The Red component. */
        float green;    /*!< The Green component. */
        float blue;     /*!< The Blue component. */
        float alpha;    /*!< The Alpha component. */
    };

    namespace colors {
        //! Constructs a Color object that represents Opaque White.
        inline constexpr Color opaqueWhite() noexcept {
            auto out = Color {};
            out.red = 1.0F;
            out.green = 1.0F;
            out.blue = 1.0F;
            out.alpha = 1.0F;

            return out;
        }

        //! Constructs a Color object that represents transparent White.
        inline constexpr Color transparentWhite() noexcept {
            auto out = Color {};
            out.red = 1.0F;
            out.green = 1.0F;
            out.blue = 1.0F;

            return out;
        }

        //! Constructs a Color object that represents transparent Black.
        inline constexpr Color transparentBlack() noexcept {
            return Color {};
        }

        //! Constructs a Color object that represents opaque Black.
        inline constexpr Color opaqueBlack() noexcept {
            auto out = Color {};
            out.alpha = 1.0F;

            return out;
        }
    }
}
