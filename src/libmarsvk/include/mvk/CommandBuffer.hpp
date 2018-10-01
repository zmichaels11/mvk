#pragma once

#include "volk.h"

#include <memory>

#include "mvk/CommandBufferUsageFlag.hpp"
#include "mvk/CommandBufferLevel.hpp"
#include "mvk/ShaderStage.hpp"

namespace mvk {
    class CommandPool;
    class DescriptorSet;
    class Device;
    class Pipeline;

    class CommandBuffer {
        CommandPool * _pool;
        CommandBufferLevel _level;
        VkCommandBuffer _handle;

    public:
        CommandBuffer(CommandPool * pool, CommandBufferLevel level, VkCommandBuffer handle) :
            _pool(pool),
            _level(level),
            _handle(handle) {}

        ~CommandBuffer();

        Device * getDevice() const;

        void reset(unsigned int flags = 0);

        inline CommandPool * getCommandPool() const noexcept {
            return _pool;
        }

        inline VkCommandBuffer getHandle() const noexcept {
            return _handle;
        }

        inline CommandBufferLevel getLevel() const noexcept {
            return _level;
        }

        void begin(CommandBufferUsageFlag flags);

        void bindDescriptorSet(const Pipeline * pipeline, int firstSet, const DescriptorSet * descriptorSet);

        template<class PipelineT>
        inline void bindDescriptorSet(const std::unique_ptr<PipelineT>& pipeline, int firstSet, const DescriptorSet * descriptorSet) {
            bindDescriptorSet(pipeline.get(), firstSet, descriptorSet);
        }

        void bindPipeline(const Pipeline * pipeline);

        template<class PipelineT>
        inline void bindPipeline(std::unique_ptr<PipelineT>& pipeline) {
            bindPipeline(pipeline.get());
        }

        void dispatch(unsigned int groupsX, unsigned int groupsY = 1, unsigned int groupsZ = 1);

        void end();
        //void pushConstants(const Pipeline * pipeline, ShaderStage stages, int offset, const void * data);
    };
}
