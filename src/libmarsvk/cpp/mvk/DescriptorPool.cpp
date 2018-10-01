#include "mvk/DescriptorPool.hpp"

#include <stdexcept>

#include "mvk/DescriptorSetLayout.hpp"
#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    DescriptorPool::~DescriptorPool() {
        auto pDevice = getDevice();

        for (const auto& set : _allocatedDescriptorSets) {
            const auto& pool = _pools[set->getPoolIndex()];
            auto handle = set->getHandle();

            vkFreeDescriptorSets(pDevice->getHandle(), pool->_handle, 1, &handle);
        }

        for (const auto& pool : _pools) {
            vkDestroyDescriptorPool(pDevice->getHandle(), pool->_handle, nullptr);
        }
    }

    void DescriptorPool::releaseDescriptorSet(DescriptorSet * pSet) {
        auto& pool = _pools[pSet->getPoolIndex()];
        auto setHandle = pSet->getHandle();

        vkFreeDescriptorSets(getDevice()->getHandle(), pool->_handle, 1, &setHandle);

        pool->_allocatedSets -= 1;

        for (auto it = _allocatedDescriptorSets.begin(); it != _allocatedDescriptorSets.end(); ++it) {
            if (it->get() == pSet) {
                _allocatedDescriptorSets.erase(it);
                return;
            }
        }

        throw std::runtime_error("Attempted to release DescriptorSet that either doesn't belong or has already been removed from this DescriptorPool!");
    }

    DescriptorSet * DescriptorPool::allocate() {
        auto setLayout = _descriptorSetLayout->getHandle();

        VkDescriptorSetAllocateInfo descriptorSetAI {};
        descriptorSetAI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAI.pSetLayouts = &setLayout;
        descriptorSetAI.descriptorSetCount = 1;

        Pool * pSelectedPool = nullptr;

        for (auto& pool : _pools) {
            if (pool->_allocatedSets < _info.maxSets) {
                pSelectedPool = pool.get();
                break;
            }
        }

        auto pDevice = getDevice();
        VkDescriptorSet handle = VK_NULL_HANDLE;

        if (nullptr == pSelectedPool) {
            pSelectedPool = allocatePool(pDevice);
        }

        descriptorSetAI.descriptorPool = pSelectedPool->_handle;

        Util::vkAssert(vkAllocateDescriptorSets(pDevice->getHandle(), &descriptorSetAI, &handle));

        pSelectedPool->_allocatedSets += 1;

        auto ptr = std::make_unique<DescriptorSet> (this, pSelectedPool->_index, handle);
        auto out = ptr.get();

        _allocatedDescriptorSets.insert(std::move(ptr));

        return out;
    }

    DescriptorPool::Pool * DescriptorPool::allocatePool(Device * pDevice) {
        VkDescriptorPoolCreateInfo descriptorPoolCI {};
        descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.flags = _info.flags | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptorPoolCI.pPoolSizes = _info.poolSizes.data();
        descriptorPoolCI.poolSizeCount = _info.poolSizes.size();
        descriptorPoolCI.maxSets = _info.maxSets;

        VkDescriptorPool handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateDescriptorPool(pDevice->getHandle(), &descriptorPoolCI, nullptr, &handle));

        auto index = static_cast<int> (_pools.size());
        auto ptr = std::make_unique<Pool> (handle, index);
        auto out = ptr.get();

        _pools.push_back(std::move(ptr));
    }

    Device * DescriptorPool::getDevice() const {
        return _descriptorSetLayout->getDevice();
    }
}
