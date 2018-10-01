#pragma once

#include "volk.h"

#include <memory>
#include <vector>

#include "mvk/DescriptorPool.hpp"
#include "mvk/DescriptorSet.hpp"
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
            unsigned int flags;
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

        inline VkDescriptorSetLayout getHandle() const noexcept {
            return _handle;
        }

        inline const CreateInfo getInfo() const noexcept {
            return _info;
        }

        inline DescriptorSetLayoutCache * getDescriptorSetLayoutCache() const noexcept {
            return _cache;
        }

        inline DescriptorPool * getDescriptorPool() const noexcept {
            return _pool.get();
        }

        inline DescriptorSet * allocate() {
            return _pool->allocate();
        }
    };

    inline bool operator== (const DescriptorSetLayout::Binding& lhs, const DescriptorSetLayout::Binding& rhs) {
        return lhs.binding == rhs.binding 
            && lhs.descriptorType == rhs.descriptorType 
            && lhs.descriptorCount == rhs.descriptorCount 
            && lhs.stages == rhs.stages;
    }

    inline bool operator== (const DescriptorSetLayout::CreateInfo& lhs, const DescriptorSetLayout::CreateInfo& rhs) {
        return lhs.flags == rhs.flags 
                && lhs.bindings == rhs.bindings;
    }
}
