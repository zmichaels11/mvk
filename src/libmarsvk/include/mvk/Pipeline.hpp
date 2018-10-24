#pragma once

#include <cstddef>

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
        virtual PipelineBindPoint getBindPoint() const noexcept = 0;

        virtual void release() = 0;

        virtual Device * getDevice() const noexcept = 0;

        virtual PipelineCache * getPipelineCache() const noexcept = 0;

        virtual PipelineLayout * getPipelineLayout() const noexcept = 0;

        virtual DescriptorSetLayout * getDescriptorSetLayout(std::ptrdiff_t index) const noexcept = 0;

        virtual std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const noexcept = 0;

        virtual VkPipeline getHandle() const noexcept = 0;
    };
}
