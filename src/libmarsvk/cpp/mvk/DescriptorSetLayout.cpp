#include "mvk/DescriptorSetLayout.hpp"

#include <map>

#include "mvk/DescriptorSetLayoutCache.hpp"
#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        constexpr unsigned int MAX_SETS = 32;
    }

    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayoutCache * cache, const CreateInfo& info) {
        _cache = cache;
        _info = info;

        auto pDevice = cache->getDevice();

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI {};
        descriptorSetLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCI.flags = _info.flags;

        auto pBindings = std::vector<VkDescriptorSetLayoutBinding>();
        pBindings.reserve(_info.bindings.size());

        for (const auto& binding : _info.bindings) {
            VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
            descriptorSetLayoutBinding.binding = static_cast<uint32_t> (binding.binding);
            descriptorSetLayoutBinding.descriptorCount = static_cast<uint32_t> (binding.descriptorCount);
            descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType> (binding.descriptorType);
            descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags> (binding.stages);

            pBindings.push_back(descriptorSetLayoutBinding);
        }

        descriptorSetLayoutCI.pBindings = pBindings.data();
        descriptorSetLayoutCI.bindingCount = pBindings.size();

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateDescriptorSetLayout(pDevice->getHandle(), &descriptorSetLayoutCI, nullptr, &_handle));

        auto poolSizesByType = std::map<DescriptorType, unsigned int>();

        for (auto& binding : _info.bindings) {
            poolSizesByType[binding.descriptorType] += binding.descriptorCount;
        }

        DescriptorPool::CreateInfo poolCI {};
        poolCI.maxSets = MAX_SETS;
        poolCI.poolSizes = std::vector<VkDescriptorPoolSize>();
        poolCI.flags = 0;
        
        for (const auto& sizeByType : poolSizesByType) {
            VkDescriptorPoolSize poolSize {};
            poolSize.type = static_cast<VkDescriptorType> (sizeByType.first);
            poolSize.descriptorCount = sizeByType.second * MAX_SETS;

            poolCI.poolSizes.push_back(poolSize);
        }

        _pool = std::make_unique<DescriptorPool> (poolCI, this);
    }

    DescriptorSetLayout::~DescriptorSetLayout() {
        _pool.reset();
        vkDestroyDescriptorSetLayout(getDevice()->getHandle(), _handle, nullptr);
    }

    void DescriptorSetLayout::release() {
        _cache->releaseDescriptorSetLayout(this);
    }

    Device * DescriptorSetLayout::getDevice() const {
        return _cache->getDevice();
    }
}
