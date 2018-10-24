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
        computePipelineCI.flags = static_cast<VkPipelineCreateFlags> (createInfo.flags);
        computePipelineCI.layout = _layout->getHandle();
        computePipelineCI.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        computePipelineCI.stage.stage = static_cast<VkShaderStageFlagBits> (createInfo.stage.stage);
        computePipelineCI.stage.pName = createInfo.stage.name.c_str();
        computePipelineCI.stage.module = pDevice->getShaderModule(createInfo.stage.moduleInfo)->getHandle();
        computePipelineCI.stage.flags = static_cast<VkPipelineShaderStageCreateFlags> (createInfo.stage.flags);

        Util::vkAssert(vkCreateComputePipelines(pDevice->getHandle(), cache->getHandle(), 1, &computePipelineCI, nullptr, &_handle));
    }

    ComputePipeline::~ComputePipeline() noexcept {
        _layout->release();
        vkDestroyPipeline(getDevice()->getHandle(), _handle, nullptr);
    }

    ComputePipeline& ComputePipeline::operator= (ComputePipeline&& from) noexcept {
        std::swap(_cache, from._cache);
        std::swap(_info, from._info);
        std::swap(_layout, from._layout);
        std::swap(_handle, from._handle);

        return *this;
    }

    PipelineBindPoint ComputePipeline::getBindPoint() const noexcept {
        return PipelineBindPoint::COMPUTE;
    }

    void ComputePipeline::release() {
        
    }

    Device * ComputePipeline::getDevice() const noexcept {
        return _cache->getDevice();
    }

    PipelineCache * ComputePipeline::getPipelineCache() const noexcept {
        return _cache;
    }

    PipelineLayout * ComputePipeline::getPipelineLayout() const noexcept {
        return _layout;
    }

    DescriptorSetLayout * ComputePipeline::getDescriptorSetLayout(std::ptrdiff_t index) const noexcept {
        return _layout->getDescriptorSetLayout(index);
    }

    std::vector<DescriptorSetLayout * > ComputePipeline::getDescriptorSetLayouts() const noexcept {
        return _layout->getDescriptorSetLayouts();
    }

    VkPipeline ComputePipeline::getHandle() const noexcept {
        return _handle;
    }
}
