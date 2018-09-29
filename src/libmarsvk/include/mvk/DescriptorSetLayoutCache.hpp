#pragma once

#include <vector>

#include "mvk/DescriptorSetLayout.hpp"

namespace mvk {
    class Device;

    class DescriptorSetLayoutCache {
        Device * _device;
        std::vector<DescriptorSetLayout> _layouts;

    public:
        DescriptorSetLayoutCache(Device * device) :
            _device(device) {}

        DescriptorSetLayout * allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo);

        void releaseDescriptorSetLayout(DescriptorSetLayout * layout);

        inline Device * getDevice() const {
            return _device;
        }
    };
}
