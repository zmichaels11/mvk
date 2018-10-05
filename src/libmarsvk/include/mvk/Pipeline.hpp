#pragma once

#include "volk.h"

#include <vector>

#include "mvk/PipelineBindPoint.hpp"

namespace mvk {
    class DescriptorSetLayout;
    class Device;
    class PipelineCache;
    class PipelineLayout;

    class Pipeline {
    public:
        virtual PipelineBindPoint getBindPoint() const = 0;

        virtual void release() = 0;

        virtual Device * getDevice() const = 0;

        virtual PipelineCache * getPipelineCache() const = 0;

        virtual PipelineLayout * getPipelineLayout() const = 0;

        virtual DescriptorSetLayout * getDescriptorSetLayout(int index) const = 0;

        virtual std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const = 0;

        virtual VkPipeline getHandle() const = 0;
    };
}
