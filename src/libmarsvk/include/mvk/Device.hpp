#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "mvk/Buffer.hpp"
#include "mvk/DescriptorSetLayoutCache.hpp"
#include "mvk/Device.hpp"
#include "mvk/FencePool.hpp"
#include "mvk/MemoryUsage.hpp"
#include "mvk/PipelineLayoutCache.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/SemaphorePool.hpp"
#include "mvk/ShaderModule.hpp"

namespace mvk {
    class PhysicalDevice;

    class Device {
        PhysicalDevice * _physicalDevice;
        VkDevice _handle;
        std::set<std::string> _enabledExtensions;
        std::unique_ptr<QueueFamily[]> _queueFamilies;
        std::uint32_t _queueFamilyCount;
        std::vector<std::unique_ptr<ShaderModule>> _shaderCache;
        std::unique_ptr<FencePool> _fencePool;
        std::unique_ptr<SemaphorePool> _semaphorePool;
        std::unique_ptr<DescriptorSetLayoutCache> _descriptorSetLayoutCache;
        std::unique_ptr<PipelineLayoutCache> _pipelineLayoutCache;
        VmaAllocator _allocator;

    public:
        Device() : 
            _handle(VK_NULL_HANDLE),
            _physicalDevice(nullptr) {}

        Device(PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions);

        ~Device();

        Device(const Device&) = delete;

        Device(Device&&) = default;

        Device& operator=(const Device&) = delete;

        Device& operator=(Device&&) = default;

        void waitIdle();

        inline const std::set<std::string>& getEnabledExtensions() const noexcept {
            return _enabledExtensions;
        }

        inline PhysicalDevice * getPhysicalDevice() const noexcept {
            return _physicalDevice;
        }

        inline QueueFamily * getQueueFamily(std::ptrdiff_t index) const noexcept {
            return _queueFamilies.get() + index;
        }

        inline std::uint32_t getQueueFamilyCount() const noexcept {
            return _queueFamilyCount;
        }

        inline VkDevice getHandle() const {
            return _handle;
        }

        inline Fence * acquireFence() {
            return _fencePool->acquireFence();
        }

        inline Semaphore * acquireSemaphore() {
            return _semaphorePool->acquireSemaphore();
        }

        inline VmaAllocator& getMemoryAllocator() {
            return _allocator;
        }

        inline std::unique_ptr<Buffer> createBuffer(const Buffer::CreateInfo& info, MemoryUsage memoryUsage) {
            return std::make_unique<Buffer> (this, info, memoryUsage);
        }

        inline DescriptorSetLayout * allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo) {
            return _descriptorSetLayoutCache->allocateDescriptorSetLayout(createInfo);
        }

        inline PipelineLayout * allocatePipelineLayout(const PipelineLayout::CreateInfo& createInfo) {
            return _pipelineLayoutCache->allocatePipelineLayout(createInfo);
        }

        std::vector<QueueFamily *> getQueueFamilies() const;

        ShaderModule * getShaderModule(const ShaderModule::CreateInfo& createInfo);
    };
}
