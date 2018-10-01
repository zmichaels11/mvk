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

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline VkPipelineLayout getHandle() const noexcept {
            return _handle;
        }

        Device * getDevice() const;

        void release();
    };

    inline bool operator== (const PipelineLayout::CreateInfo& lhs, const PipelineLayout::CreateInfo& rhs) {
        return lhs.flags == rhs.flags
                && lhs.pushConstantRanges == rhs.pushConstantRanges
                && lhs.setLayoutInfos == rhs.setLayoutInfos;
    }
}
