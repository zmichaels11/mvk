#pragma once

#include "volk.h"

#include "mvk/Pipeline.hpp"
#include "mvk/PipelineLayout.hpp"
#include "mvk/PipelineShaderStageCreateInfo.hpp"

namespace mvk {
    class Device;
    class PipelineCache;
    class DescriptorSetLayout;

    class ComputePipeline : public virtual Pipeline {
    public:
        struct CreateInfo {
            unsigned int flags;
            PipelineShaderStageCreateInfo stage;
            PipelineLayout::CreateInfo layoutInfo;
        };

    private:
        CreateInfo _info;
        VkPipeline _handle;
        PipelineCache * _cache;
        PipelineLayout * _layout;

    public:
        ComputePipeline(PipelineCache * cache, const CreateInfo& createInfo);

        ~ComputePipeline();

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        virtual PipelineBindpoint getBindpoint() const;

        virtual void release();

        virtual Device * getDevice() const;

        virtual PipelineCache * getPipelineCache() const;

        virtual PipelineLayout * getPipelineLayout() const;

        virtual DescriptorSetLayout * getDescriptorSetLayout(int index) const;

        virtual std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const;

        virtual VkPipeline getHandle() const;
    };
}
