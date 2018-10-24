#include "mvk/GraphicsPipeline.hpp"

#include <iostream>
#include <memory>

#include "mvk/Device.hpp"
#include "mvk/PipelineCache.hpp"
#include "mvk/RenderPass.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    GraphicsPipeline::GraphicsPipeline(PipelineCache * cache, const GraphicsPipeline::CreateInfo& createInfo, const RenderPass * renderPass) {
        _cache = cache;
        _info = createInfo;

        auto pDevice = cache->getDevice();
        auto stages = std::vector<VkPipelineShaderStageCreateInfo> ();
        stages.reserve(createInfo.stages.size());

        for (const auto& stage : createInfo.stages) {
            auto info = VkPipelineShaderStageCreateInfo {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            info.stage = static_cast<VkShaderStageFlagBits> (stage.stage);
            info.pName = stage.name.c_str();
            info.module = pDevice->getShaderModule(stage.moduleInfo)->getHandle();
            info.flags = stage.flags;

            stages.push_back(info);
        }

        _layout = pDevice->allocatePipelineLayout(createInfo.layoutInfo);

        auto multisampleState = VkPipelineMultisampleStateCreateInfo {};
        multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleState.flags = createInfo.multisampleState.flags;
        multisampleState.rasterizationSamples = static_cast<VkSampleCountFlagBits> (createInfo.multisampleState.rasterizationSamples);
        multisampleState.sampleShadingEnable = createInfo.multisampleState.sampleShadingEnable ? VK_TRUE : VK_FALSE;
        multisampleState.minSampleShading = createInfo.multisampleState.minSampleShading ? VK_TRUE : VK_FALSE;
        multisampleState.pSampleMask = reinterpret_cast<VkSampleMask * > (createInfo.multisampleState.pSampleMask);
        multisampleState.alphaToCoverageEnable = createInfo.multisampleState.alphaToCoverageEnable ? VK_TRUE : VK_FALSE;
        multisampleState.alphaToOneEnable = createInfo.multisampleState.alphaToOneEnable ? VK_TRUE : VK_FALSE;

        auto rasterizationState = VkPipelineRasterizationStateCreateInfo {};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.flags = createInfo.rasterizationState.flags;
        rasterizationState.depthClampEnable = createInfo.rasterizationState.depthClampEnable ? VK_TRUE : VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = createInfo.rasterizationState.rasterizationDiscardEnable ? VK_TRUE : VK_FALSE;
        rasterizationState.polygonMode = static_cast<VkPolygonMode> (createInfo.rasterizationState.polygonMode);
        rasterizationState.cullMode = static_cast<VkCullModeFlags> (createInfo.rasterizationState.cullMode);
        rasterizationState.frontFace = static_cast<VkFrontFace> (createInfo.rasterizationState.frontFace);
        rasterizationState.depthBiasEnable = createInfo.rasterizationState.depthBiasEnable ? VK_TRUE : VK_FALSE;
        rasterizationState.depthBiasConstantFactor = createInfo.rasterizationState.depthBiasConstantFactor;
        rasterizationState.depthBiasClamp = createInfo.rasterizationState.depthBiasClamp;
        rasterizationState.depthBiasSlopeFactor = createInfo.rasterizationState.depthBiasSlopeFactor;
        rasterizationState.lineWidth = createInfo.rasterizationState.lineWidth;

        auto depthStencilState = VkPipelineDepthStencilStateCreateInfo {};
        depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilState.flags = createInfo.depthStencilState.flags;
        depthStencilState.depthTestEnable = createInfo.depthStencilState.depthTestEnable ? VK_TRUE : VK_FALSE;
        depthStencilState.depthWriteEnable = createInfo.depthStencilState.depthWriteEnable ? VK_TRUE : VK_FALSE;
        depthStencilState.depthCompareOp = static_cast<VkCompareOp> (createInfo.depthStencilState.depthCompareOp);
        depthStencilState.stencilTestEnable = createInfo.depthStencilState.stencilTestEnable ? VK_TRUE : VK_FALSE;
        depthStencilState.minDepthBounds = createInfo.depthStencilState.minDepthBounds;
        depthStencilState.maxDepthBounds = createInfo.depthStencilState.maxDepthBounds;

        auto deserializeStencilState = [](VkStencilOpState& dst, const StencilOpState& src) {
            dst.failOp = static_cast<VkStencilOp> (src.failOp);
            dst.passOp = static_cast<VkStencilOp> (src.passOp);
            dst.depthFailOp = static_cast<VkStencilOp> (src.depthFailOp);
            dst.compareOp = static_cast<VkCompareOp> (src.compareOp);
            dst.compareMask = src.compareMask;
            dst.writeMask = src.writeMask;
            dst.reference = src.reference;
        };

        deserializeStencilState(depthStencilState.front, createInfo.depthStencilState.front);
        deserializeStencilState(depthStencilState.back, createInfo.depthStencilState.back);        

        auto pDynamicStates = std::vector<VkDynamicState> ();
        pDynamicStates.reserve(2);
        pDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
        pDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);

        auto dynamicState = VkPipelineDynamicStateCreateInfo {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = pDynamicStates.size();
        dynamicState.pDynamicStates = pDynamicStates.data();

        auto inputAssemblyState = VkPipelineInputAssemblyStateCreateInfo {};
        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.flags = createInfo.inputAssemblyState.flags;
        inputAssemblyState.topology = static_cast<VkPrimitiveTopology> (createInfo.inputAssemblyState.topology);
        inputAssemblyState.primitiveRestartEnable = createInfo.inputAssemblyState.primitiveRestartEnable;

        auto viewportState = VkPipelineViewportStateCreateInfo {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        auto bindingDescriptions = std::vector<VkVertexInputBindingDescription> ();
        bindingDescriptions.reserve(createInfo.vertexInputState.vertexBindingDescriptions.size());

        for (const auto& binding : createInfo.vertexInputState.vertexBindingDescriptions) {
            auto vkb = VkVertexInputBindingDescription {};
            vkb.binding = static_cast<uint32_t> (binding.binding);
            vkb.inputRate = static_cast<VkVertexInputRate> (binding.inputRate);
            vkb.stride = static_cast<uint32_t> (binding.stride);

            bindingDescriptions.push_back(vkb);
        }

        auto attributeDescriptions = std::vector<VkVertexInputAttributeDescription> ();
        attributeDescriptions.reserve(createInfo.vertexInputState.vertexAttributeDescriptions.size());

        for (const auto& attrib : createInfo.vertexInputState.vertexAttributeDescriptions) {
            auto vka = VkVertexInputAttributeDescription {};
            vka.binding = static_cast<uint32_t> (attrib.binding);
            vka.format = static_cast<VkFormat> (attrib.format);
            vka.location = static_cast<uint32_t> (attrib.location);
            vka.offset = static_cast<uint32_t> (attrib.offset);

            attributeDescriptions.push_back(vka);
        }

        auto vertexInputState = VkPipelineVertexInputStateCreateInfo {};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.flags = createInfo.vertexInputState.flags;
        vertexInputState.vertexBindingDescriptionCount = bindingDescriptions.size();
        vertexInputState.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputState.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

        auto tessellationState = VkPipelineTessellationStateCreateInfo {};
        tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tessellationState.flags = createInfo.tessellationState.flags;
        tessellationState.patchControlPoints = static_cast<uint32_t> (createInfo.tessellationState.patchControlPoints);

        auto attachments = std::vector<VkPipelineColorBlendAttachmentState> ();
        attachments.reserve(createInfo.colorBlendState.attachments.size());

        for (const auto& attachment : createInfo.colorBlendState.attachments) {
            auto vkcb = VkPipelineColorBlendAttachmentState {};
            vkcb.blendEnable = attachment.blendEnable ? VK_TRUE : VK_FALSE;
            vkcb.srcColorBlendFactor = static_cast<VkBlendFactor> (attachment.srcColorBlendFactor);
            vkcb.dstColorBlendFactor = static_cast<VkBlendFactor> (attachment.dstColorBlendFactor);
            vkcb.colorBlendOp = static_cast<VkBlendOp> (attachment.colorBlendOp);
            vkcb.srcAlphaBlendFactor = static_cast<VkBlendFactor> (attachment.srcAlphaBlendFactor);
            vkcb.dstAlphaBlendFactor = static_cast<VkBlendFactor> (attachment.dstAlphaBlendFactor);
            vkcb.alphaBlendOp = static_cast<VkBlendOp> (attachment.alphaBlendOp);
            vkcb.colorWriteMask = static_cast<VkColorComponentFlags> (attachment.colorWriteMask);

            attachments.push_back(vkcb);
        }

        auto colorBlendState = VkPipelineColorBlendStateCreateInfo {};
        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.flags = createInfo.colorBlendState.flags;
        colorBlendState.logicOpEnable = createInfo.colorBlendState.logicOpEnable ? VK_TRUE : VK_FALSE;
        colorBlendState.logicOp = static_cast<VkLogicOp> (createInfo.colorBlendState.logicOp);
        colorBlendState.attachmentCount = attachments.size();
        colorBlendState.pAttachments = attachments.data();
        colorBlendState.blendConstants[0] = createInfo.colorBlendState.blendConstants.red;
        colorBlendState.blendConstants[1] = createInfo.colorBlendState.blendConstants.green;
        colorBlendState.blendConstants[2] = createInfo.colorBlendState.blendConstants.blue;
        colorBlendState.blendConstants[3] = createInfo.colorBlendState.blendConstants.alpha;

        auto graphicsPipelineCI = VkGraphicsPipelineCreateInfo {};
        graphicsPipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCI.flags = createInfo.flags;
        graphicsPipelineCI.stageCount = stages.size();
        graphicsPipelineCI.pStages = stages.data();
        graphicsPipelineCI.pVertexInputState = &vertexInputState;
        graphicsPipelineCI.pInputAssemblyState = &inputAssemblyState;
        graphicsPipelineCI.pTessellationState = &tessellationState;
        graphicsPipelineCI.pViewportState = &viewportState;
        graphicsPipelineCI.pRasterizationState = &rasterizationState;
        graphicsPipelineCI.pMultisampleState = &multisampleState;
        graphicsPipelineCI.pDepthStencilState = &depthStencilState;
        graphicsPipelineCI.pColorBlendState = &colorBlendState;
        graphicsPipelineCI.pDynamicState = &dynamicState;
        graphicsPipelineCI.layout = _layout->getHandle();
        graphicsPipelineCI.renderPass = renderPass->getHandle();
        graphicsPipelineCI.subpass = createInfo.subpass;

        _handle = VK_NULL_HANDLE;
        Util::vkAssert(vkCreateGraphicsPipelines(pDevice->getHandle(), cache->getHandle(), 1, &graphicsPipelineCI, nullptr, &_handle));
    }

    GraphicsPipeline::~GraphicsPipeline() noexcept {
        _layout->release();
        vkDestroyPipeline(getDevice()->getHandle(), _handle, nullptr);
    }

    GraphicsPipeline& GraphicsPipeline::operator= (GraphicsPipeline&& from) noexcept {
        std::swap(this->_cache, from._cache);
        std::swap(this->_handle, from._handle);
        std::swap(this->_info, from._info);
        std::swap(this->_layout, from._layout);

        return *this;
    }

    PipelineBindPoint GraphicsPipeline::getBindPoint() const noexcept {
        return PipelineBindPoint::GRAPHICS;
    }

    void GraphicsPipeline::release() {

    }

    Device * GraphicsPipeline::getDevice() const noexcept {
        return _cache->getDevice();
    }

    PipelineCache * GraphicsPipeline::getPipelineCache() const noexcept {
        return _cache;
    }

    PipelineLayout * GraphicsPipeline::getPipelineLayout() const noexcept {
        return _layout;
    }

    DescriptorSetLayout * GraphicsPipeline::getDescriptorSetLayout(std::ptrdiff_t index) const noexcept {
        return _layout->getDescriptorSetLayout(index);
    }

    std::vector<DescriptorSetLayout * > GraphicsPipeline::getDescriptorSetLayouts() const noexcept {
        return _layout->getDescriptorSetLayouts();
    }

    VkPipeline GraphicsPipeline::getHandle() const noexcept {
        return _handle;
    }
}
