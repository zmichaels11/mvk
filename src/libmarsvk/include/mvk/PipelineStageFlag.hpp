#pragma once

#include "volk.h"

namespace mvk {
    enum class PipelineStageFlag : unsigned int {
        HOST = VK_PIPELINE_STAGE_HOST_BIT,
        TOP_OF_PIPE = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        DRAW_INDIRECT = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
        VERTEX_INPUT = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        VERTEX_SHADER = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        TESSELLATION_CONTROL_SHADER = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
        TESSELLATION_EVALUATION_SHADER = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
        GEOMETRY_SHADER = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
        FRAGMENT_SHADER = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        EARLY_FRAGMENT_TESTS = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        LATE_FRAGMENT_TESTS = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        COLOR_ATTACHMENT_OUTPUT_BIT = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        COMPUTE_SHADER = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        TRANSFER = VK_PIPELINE_STAGE_TRANSFER_BIT,
        BOTTOM_OF_PIPE = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
    };

    constexpr PipelineStageFlag operator| (PipelineStageFlag lhs, PipelineStageFlag rhs) {
        return static_cast<PipelineStageFlag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }
    
    constexpr PipelineStageFlag operator& (PipelineStageFlag lhs, PipelineStageFlag rhs) {
        return static_cast<PipelineStageFlag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}
