#pragma once

namespace mvk {
    //! A 2D size value
    struct Extent2D {
        int width;      /*!< The width of the extent */
        int height;     /*!< The height of the extent */
    };

    //! Compares two extents.
    /*!
        Compares two Extent2D objects. They are only considered equal if both their widths
        and their heights are equal.

        \param lhs is the left-hand side of a comparison function.
        \param rhs is the right-hand side of a comparison function.
        \return true if both extents are equal.
    */
    inline constexpr bool operator== (const Extent2D& lhs, const Extent2D& rhs) noexcept {
        return lhs.width == rhs.width && lhs.height == rhs.height;
    }
}
