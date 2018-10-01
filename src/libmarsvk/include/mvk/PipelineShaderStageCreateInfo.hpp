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
    };
}
