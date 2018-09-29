#pragma once

#include "volk.h"

#include <memory>
#include <vector>

#include "mvk/DescriptorPool.hpp"
#include "mvk/DescriptorType.hpp"
#include "mvk/ShaderStage.hpp"

namespace mvk {
    class DescriptorSetLayoutCache;
    class Device;

    class DescriptorSetLayout {
    public:
        struct Binding {
            unsigned int binding;
            DescriptorType descriptorType;
            unsigned int descriptorCount;
            ShaderStage stages;
        };

        struct CreateInfo {
            int flags;
            std::vector<Binding> bindings;
        };

    private:
        VkDescriptorSetLayout _handle;
        CreateInfo _info;
        DescriptorSetLayoutCache * _cache;
        std::unique_ptr<DescriptorPool> _pool;

    public:
        DescriptorSetLayout() :
            _handle(nullptr) {}

        DescriptorSetLayout(DescriptorSetLayoutCache * cache, const CreateInfo& info);

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;

        DescriptorSetLayout(DescriptorSetLayout&&) = default;

        ~DescriptorSetLayout();

        DescriptorSetLayout& operator= (const DescriptorSetLayout&) = delete;

        DescriptorSetLayout& operator= (DescriptorSetLayout&&) = delete;

        void release();

        Device * getDevice() const;

        inline DescriptorSetLayoutCache * getDescriptorSetLayoutCache() const noexcept {
            return _cache;
        }
    };
}
