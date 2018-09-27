#pragma once

#include <cstddef>

namespace mvk {
    struct Buffercopy {
        std::ptrdiff_t srcOffset;
        std::ptrdiff_t dstOffset;
        std::size_t size;
    };
}
