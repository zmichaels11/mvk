#pragma once

namespace mvk {
    struct Extent3D {
        int width;
        int height;
        int depth;
    };

    inline constexpr bool operator== (const Extent3D& lhs, const Extent3D& rhs) {
        return lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth;
    }
}
