#pragma once

#include "mvk/VertexInputRate.hpp"

namespace mvk {
    struct VertexInputBindingDescription {
        int binding;
        int stride;
        VertexInputRate inputRate;
    };
}
