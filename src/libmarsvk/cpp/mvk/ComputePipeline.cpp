#include "mvk/ComputePipeline.hpp"

#include "mvk/Device.hpp"
#include "mvk/PipelineCache.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    ComputePipeline::ComputePipeline(PipelineCache * cache, const ComputePipeline::CreateInfo& createInfo) {
        _cache = cache;
        _info = createInfo;

        auto pDevice = cache->getDevice();
        
        _layout = pDevice->allocatePipelineLayout(createInfo.layoutInfo);

        auto computePipelineCI = VkComputePipelineCreateInfo {};
        computePipelineCI.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCI.flags = createInfo.flags;
        computePipelineCI.layout = _layout->getHandle();
        computePipelineCI.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        computePipelineCI.stage.stage = static_cast<VkShaderStageFlagBits> (createInfo.stage.stage);
        computePipelineCI.stage.pName = createInfo.stage.name.c_str();
        computePipelineCI.stage.module = pDevice->getShaderModule(createInfo.stage.moduleInfo)->getHandle();
        computePipelineCI.stage.flags = createInfo.stage.flags;

        _handle = VK_NULL_HANDLE;
        Util::vkAssert(vkCreateComputePipelines(pDevice->getHandle(), cache->getHandle(), 1, &computePipelineCI, nullptr, &_handle));
    }

    ComputePipeline::~ComputePipeline() {
        _layout->release();
        vkDestroyPipeline(getDevice()->getHandle(), _handle, nullptr);
    }

    PipelineBindpoint ComputePipeline::getBindpoint() const {
        return PipelineBindpoint::COMPUTE;
    }

    void ComputePipeline::release() {
        
    }

    Device * ComputePipeline::getDevice() const {
        return _cache->getDevice();
    }

    PipelineCache * ComputePipeline::getPipelineCache() const {
        return _cache;
    }

    PipelineLayout * ComputePipeline::getPipelineLayout() const {
        return _layout;
    }

    DescriptorSetLayout * ComputePipeline::getDescriptorSetLayout(std::ptrdiff_t index) const {
        return _layout->getDescriptorSetLayout(index);
    }

    std::vector<DescriptorSetLayout * > ComputePipeline::getDescriptorSetLayouts() const {
        return _layout->getDescriptorSetLayouts();
    }

    VkPipeline ComputePipeline::getHandle() const {
        return _handle;
    }
}
