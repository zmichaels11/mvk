#pragma once

#include <cstdint>

#include "volk.h"

#include "mvk/DescriptorType.hpp"

namespace mvk {
    class Buffer;
    class Device;
    class DescriptorPool;

    class DescriptorSet {
        VkDescriptorSet _handle;
        DescriptorPool * _pool;
        int _poolIndex;

    public:
        DescriptorSet() :
            _handle(VK_NULL_HANDLE),
            _pool(nullptr),
            _poolIndex(-1) {}

        DescriptorSet(DescriptorPool * pool, int poolIndex, VkDescriptorSet handle) :
            _handle(handle),
            _pool(pool),
            _poolIndex(poolIndex) {}

        DescriptorSet(const DescriptorSet&) = delete;

        DescriptorSet(DescriptorSet&&) = default;

        DescriptorSet& operator= (const DescriptorSet&) = default;

        DescriptorSet& operator= (DescriptorSet&&) = default;

        inline DescriptorPool * getDescriptorPool() const {
            return _pool;
        }

        inline int getPoolIndex() const {
            return _poolIndex;
        }

        inline VkDescriptorSet getHandle() const {
            return _handle;
        }

        Device * getDevice() const;

        void release();

        void writeBuffer(
            DescriptorType type, std::uint32_t binding, 
            const Buffer * buffer, VkDeviceSize offset = 0L, VkDeviceSize range = VK_WHOLE_SIZE);
    };
}
