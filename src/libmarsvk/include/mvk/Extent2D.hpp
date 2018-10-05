#pragma once

namespace mvk {
    struct Extent2D {
        int width;
        int height;
    };

    inline constexpr bool operator== (const Extent2D& lhs, const Extent2D& rhs) {
        return lhs.width == rhs.width && lhs.height == rhs.height;
    }
}

