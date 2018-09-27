#pragma once

#include "volk.h"

namespace mvk {
    enum class ShaderStage : unsigned int {
        VERTEX = VK_SHADER_STAGE_VERTEX_BIT,
        TESSELLATION_CONTROL = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
        TESSELLATION_EVALUATION = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
        GEOMETRY = VK_SHADER_STAGE_GEOMETRY_BIT,
        FRAGMENT = VK_SHADER_STAGE_FRAGMENT_BIT,
        COMPUTE = VK_SHADER_STAGE_COMPUTE_BIT
    };

    constexpr ShaderStage operator| (ShaderStage lhs, ShaderStage rhs) {
        return static_cast<ShaderStage> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr ShaderStage operator& (ShaderStage lhs, ShaderStage rhs) {
        return static_cast<ShaderStage> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
