#include "mvk/PipelineLayout.hpp"

#include <vector> 
#include "mvk/Device.hpp"
#include "mvk/PipelineLayoutCache.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    PipelineLayout::PipelineLayout(PipelineLayoutCache * cache, const PipelineLayout::CreateInfo& createInfo) {
        _cache = cache;
        _info = createInfo;

        auto pDevice = cache->getDevice();
        auto pSetLayouts = std::vector<VkDescriptorSetLayout>();

        for (const auto& setLayoutCI : createInfo.setLayoutInfos) {
            auto pSetLayout = pDevice->allocateDescriptorSetLayout(setLayoutCI);

            _setLayouts.push_back(pSetLayout);
            pSetLayouts.push_back(pSetLayout->getHandle());
        }

        auto pushConstantRanges = std::vector<VkPushConstantRange> ();
        pushConstantRanges.reserve(createInfo.pushConstantRanges.size());

        for (const auto& range : createInfo.pushConstantRanges) {
            auto pcr = VkPushConstantRange {};
            pcr.stageFlags = static_cast<VkShaderStageFlags> (range.stages);
            pcr.offset = static_cast<uint32_t> (range.offset);
            pcr.size = static_cast<uint32_t> (range.size);

            pushConstantRanges.push_back(pcr);
        }

        auto pipelineLayoutCI = VkPipelineLayoutCreateInfo {};
        pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCI.flags = createInfo.flags;
        pipelineLayoutCI.pSetLayouts = pSetLayouts.data();
        pipelineLayoutCI.setLayoutCount = pSetLayouts.size();
        pipelineLayoutCI.pPushConstantRanges = pushConstantRanges.data();
        pipelineLayoutCI.pushConstantRangeCount = pushConstantRanges.size();

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreatePipelineLayout(pDevice->getHandle(), &pipelineLayoutCI, nullptr, &_handle));
    }

    PipelineLayout::~PipelineLayout() {
        vkDestroyPipelineLayout(getDevice()->getHandle(), _handle, nullptr);

        for (auto& setLayout : _setLayouts) {
            setLayout->release();
        }
    }

    Device * PipelineLayout::getDevice() const {
        return _cache->getDevice();
    }

    void PipelineLayout::release() {
        _cache->releasePipelineLayout(this);
    }
}
