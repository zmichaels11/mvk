#pragma once

#include "volk.h"

#include <memory>
#include <set>
#include <vector>

#include "mvk/DescriptorSet.hpp"

namespace mvk {
    class DescriptorSetLayout;
    class Device;

    class DescriptorPool {
    public:
        struct CreateInfo {
            unsigned int flags;
            unsigned int maxSets;
            std::vector<VkDescriptorPoolSize> poolSizes;
        };

    private:
        struct Pool {
            VkDescriptorPool _handle;
            int _index;
            int _allocatedSets;

            Pool(VkDescriptorPool handle, int index) :
                _handle(handle),
                _index(index),
                _allocatedSets(0) {}
        };

        CreateInfo _info;
        DescriptorSetLayout * _descriptorSetLayout;
        std::vector<std::unique_ptr<Pool>> _pools;
        std::set<std::unique_ptr<DescriptorSet>> _allocatedDescriptorSets;
    
        Pool * allocatePool(Device * device);

    public:
        DescriptorPool():
            _descriptorSetLayout(nullptr) {}

        DescriptorPool(const CreateInfo& info, DescriptorSetLayout * layout) :
            _info(info),
            _descriptorSetLayout(layout) {}
        
        DescriptorPool(const DescriptorPool&) = delete;

        DescriptorPool(DescriptorPool&&) = default;

        ~DescriptorPool();

        DescriptorPool& operator= (const DescriptorPool&) = delete;

        DescriptorPool& operator= (DescriptorPool&&) = default;

        void releaseDescriptorSet(DescriptorSet * set);

        DescriptorSet * allocate();

        Device * getDevice() const;

        inline DescriptorSetLayout * getDescriptorSetLayout() const {
            return _descriptorSetLayout;
        }
    };
}
