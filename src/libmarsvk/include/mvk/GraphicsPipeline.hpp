#pragma once

#include "volk.h"

#include <vector>

#include "mvk/Pipeline.hpp"
#include "mvk/PipelineColorBlendStateCreateInfo.hpp"
#include "mvk/PipelineDepthStencilStateCreateInfo.hpp"
#include "mvk/PipelineInputAssemblyStateCreateInfo.hpp"
#include "mvk/PipelineLayout.hpp"
#include "mvk/PipelineMultisampleStateCreateInfo.hpp"
#include "mvk/PipelineRasterizationStateCreateInfo.hpp"
#include "mvk/PipelineShaderStageCreateInfo.hpp"
#include "mvk/PipelineTessellationStateCreateInfo.hpp"
#include "mvk/PipelineVertexInputStateCreateInfo.hpp"

namespace mvk {
    class Device;
    class PipelineCache;
    class RenderPass;

    class GraphicsPipeline : public virtual Pipeline {
    public:
        struct CreateInfo {
            unsigned int flags;
            std::vector<PipelineShaderStageCreateInfo> stages;
            PipelineLayout::CreateInfo layoutInfo;
            PipelineColorBlendStateCreateInfo colorBlendState;
            PipelineDepthStencilStateCreateInfo depthStencilState;
            PipelineInputAssemblyStateCreateInfo inputAssemblyState;
            PipelineMultisampleStateCreateInfo multisampleState;
            PipelineRasterizationStateCreateInfo rasterizationState;
            PipelineTessellationStateCreateInfo tessellationState;
            PipelineVertexInputStateCreateInfo vertexInputState;
            int subpass;
        };

    private:
        VkPipeline _handle;
        CreateInfo _info;
        PipelineCache * _cache;
        PipelineLayout * _layout;

    public:
        void * userData;

        GraphicsPipeline() :
            _handle(VK_NULL_HANDLE),
            _cache(nullptr),
            _layout(nullptr) {}

        GraphicsPipeline(PipelineCache * cache, const CreateInfo& createInfo, const RenderPass * renderPass);

        GraphicsPipeline(const GraphicsPipeline&) = delete;

        GraphicsPipeline(GraphicsPipeline&&) = default;

        GraphicsPipeline& operator= (const GraphicsPipeline&) = delete;

        GraphicsPipeline& operator= (GraphicsPipeline&&) = default;

        ~GraphicsPipeline();

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
