#pragma once

#include <string>

#include "mvk/ShaderModule.hpp"
#include "mvk/ShaderStage.hpp"

namespace mvk {
    struct PipelineShaderStageCreateInfo {
        unsigned int flags;
        ShaderStage stage;
        ShaderModule::CreateInfo moduleInfo;
        std::string name;
        //specializationInfo

        static inline PipelineShaderStageCreateInfo init(mvk::ShaderStage stage, const std::string& path, const std::string& name = "main") noexcept {
            auto out = PipelineShaderStageCreateInfo {};
            out.stage = stage;
            out.moduleInfo.path = path;
            out.name = name;

            return out;
        }
    };
}
