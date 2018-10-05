#pragma once

#include "mvk/Format.hpp"

namespace mvk {
    struct VertexInputAttributeDescription {
        int location;
        int binding;
        Format format;
        int offset;
    };
}
