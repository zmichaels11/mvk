#pragma once

namespace mvk {
    struct Color {
        float red;
        float green;
        float blue;
        float alpha;
    };

    namespace colors {
        inline constexpr Color opaqueWhite() {
            auto out = Color {};
            out.red = 1.0F;
            out.green = 1.0F;
            out.blue = 1.0F;
            out.alpha = 1.0F;

            return out;
        }

        inline constexpr Color transparentWhite() {
            auto out = Color {};
            out.red = 1.0F;
            out.green = 1.0F;
            out.blue = 1.0F;

            return out;
        }

        inline constexpr Color transparentBlack() {
            return Color {};
        }

        inline constexpr Color opaqueBlack() {
            auto out = Color {};
            out.alpha = 1.0F;

            return out;
        }
    }
}
