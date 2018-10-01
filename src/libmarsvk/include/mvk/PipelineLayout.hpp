#pragma once

#include "volk.h"

#include <vector>

#include "mvk/DescriptorSetLayout.hpp"
#include "mvk/PushConstantRange.hpp"

namespace mvk {
    class Device;
    class PipelineLayoutCache;

    class PipelineLayout {
    public:
        struct CreateInfo {
            unsigned int flags;
            std::vector<PushConstantRange> pushConstantRanges;
            std::vector<DescriptorSetLayout::CreateInfo> setLayoutInfos;
        };

    private:
        CreateInfo _info;
        VkPipelineLayout _handle;
        PipelineLayoutCache * _cache;
        std::vector<DescriptorSetLayout * > _setLayouts;

    public:
        PipelineLayout(PipelineLayoutCache * cache, const CreateInfo& info);

        ~PipelineLayout();

        inline std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const noexcept {
            return _setLayouts;
        }

        inline DescriptorSetLayout * getDescriptorSetLayout(std::ptrdiff_t index) const noexcept {
            return _setLayouts[index];
        }

        inline PipelineLayoutCache * getPipelineLayoutCache() const noexcept {
            return _cache;
        }

        Device * getDevice() const;

        void release();
    };
}
