#pragma once

namespace mvk {
    //! A 3D size extent.
    struct Extent3D {
        int width;      /*!< The width of the extent. */
        int height;     /*!< The height of the extent. */
        int depth;      /*!< The depth of the extent. */
    };

    //! Compares two 3D extents
    /*!
        Two Extent3D objects are only equal when their widths, heights, and depths are equal.
        \param lhs the left-hand side of a comparison function.
        \param rhs the right-hand side of a comparison function.
        \return true if the two are equal.
    */
    inline constexpr bool operator== (const Extent3D& lhs, const Extent3D& rhs) noexcept {
        return lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth;
    }
}
