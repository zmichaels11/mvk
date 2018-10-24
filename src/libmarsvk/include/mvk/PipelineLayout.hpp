#pragma once

#include <cstddef>

#include "volk.h"

#include <utility>
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

        PipelineLayout(const PipelineLayout&) = delete;

        PipelineLayout& operator= (const PipelineLayout&) = delete;

    public:
        PipelineLayout() noexcept:
            _handle(VK_NULL_HANDLE),
            _cache(nullptr) {}

        PipelineLayout(PipelineLayoutCache * cache, const CreateInfo& info);

        PipelineLayout(PipelineLayout&& from) noexcept:
            _info(std::move(from._info)),
            _handle(std::exchange(from._handle, nullptr)),
            _cache(std::move(from._cache)),
            _setLayouts(std::move(from._setLayouts)) {}

        ~PipelineLayout() noexcept;

        PipelineLayout& operator= (PipelineLayout&& from) noexcept;

        inline std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const noexcept {
            return _setLayouts;
        }

        inline DescriptorSetLayout * getDescriptorSetLayout(std::ptrdiff_t index) const noexcept {
            return _setLayouts[index];
        }

        inline std::size_t getDescriptorSetLayoutCount() const noexcept {
            return _setLayouts.size();
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

        Device * getDevice() const noexcept;

        void release();
    };

    inline constexpr bool operator== (const PipelineLayout::CreateInfo& lhs, const PipelineLayout::CreateInfo& rhs) noexcept {
        return lhs.flags == rhs.flags
                && lhs.pushConstantRanges == rhs.pushConstantRanges
                && lhs.setLayoutInfos == rhs.setLayoutInfos;
    }
}
