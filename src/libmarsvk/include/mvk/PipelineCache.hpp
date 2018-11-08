#pragma once

#include "volk.h"

#include <memory>
#include <utility>

#include "mvk/ComputePipeline.hpp"
#include "mvk/GraphicsPipeline.hpp"

namespace mvk {
    class Device;
    class RenderPass;

    class PipelineCache {
        Device * _device;
        VkPipelineCache _handle;

        PipelineCache(const PipelineCache&) = delete;

        PipelineCache& operator= (const PipelineCache&) = delete;

    public:
        PipelineCache() noexcept:
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}
            
        PipelineCache(Device * device);

        PipelineCache(PipelineCache&& from) noexcept:
            _device(std::move(from._device)),
            _handle(std::exchange(from._handle, nullptr)) {}        

        ~PipelineCache() noexcept;

        PipelineCache& operator= (PipelineCache&& from) noexcept;

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
