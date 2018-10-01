#pragma once

#include "volk.h"

#include <memory>

#include "mvk/ComputePipeline.hpp"

namespace mvk {
    class Device;

    class PipelineCache {
        Device * _device;
        VkPipelineCache _handle;

    public:
        PipelineCache(Device * device);

        ~PipelineCache();

        inline std::unique_ptr<ComputePipeline> createPipeline(const ComputePipeline::CreateInfo& createInfo) {
            return std::make_unique<ComputePipeline> (this, createInfo);
        }

        inline Device * getDevice() const noexcept {
            return _device;
        }

        inline VkPipelineCache getHandle() const noexcept {
            return _handle;
        }
    };
}
