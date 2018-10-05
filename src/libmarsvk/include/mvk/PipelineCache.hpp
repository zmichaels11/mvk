#pragma once

#include "volk.h"

#include <memory>

#include "mvk/ComputePipeline.hpp"
#include "mvk/GraphicsPipeline.hpp"

namespace mvk {
    class Device;
    class RenderPass;

    class PipelineCache {
        Device * _device;
        VkPipelineCache _handle;

    public:
        PipelineCache(Device * device);

        ~PipelineCache();

        inline std::unique_ptr<ComputePipeline> createPipeline(const ComputePipeline::CreateInfo& createInfo) {
            return std::make_unique<ComputePipeline> (this, createInfo);
        }

        inline std::unique_ptr<GraphicsPipeline> createPipeline(const GraphicsPipeline::CreateInfo& createInfo, const RenderPass * renderPass) {
            return std::make_unique<GraphicsPipeline> (this, createInfo, renderPass);
        }

        inline Device * getDevice() const noexcept {
            return _device;
        }

        inline VkPipelineCache getHandle() const noexcept {
            return _handle;
        }
    };
}
