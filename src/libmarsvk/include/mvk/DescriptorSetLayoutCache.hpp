#pragma once

#include <memory>
#include <vector>

#include "mvk/DescriptorSetLayout.hpp"

namespace mvk {
    class Device;

    class DescriptorSetLayoutCache {
        Device * _device;

        struct Layout {
            DescriptorSetLayout instance;
            int references;

            Layout(DescriptorSetLayoutCache * pCache, const DescriptorSetLayout::CreateInfo& createInfo) :
                instance(pCache, createInfo),
                references(0) {}
        };

        std::vector<std::unique_ptr<Layout>> _layouts;

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
