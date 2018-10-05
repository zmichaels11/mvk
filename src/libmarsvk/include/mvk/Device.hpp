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
#include "mvk/ComputePipeline.hpp"
#include "mvk/DescriptorSetLayoutCache.hpp"
#include "mvk/Device.hpp"
#include "mvk/FencePool.hpp"
#include "mvk/Image.hpp"
#include "mvk/MemoryUsage.hpp"
#include "mvk/PipelineCache.hpp"
#include "mvk/PipelineLayoutCache.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/RenderPass.hpp"
#include "mvk/SemaphorePool.hpp"
#include "mvk/ShaderModule.hpp"
#include "mvk/Swapchain.hpp"

namespace mvk {
    class PhysicalDevice;

    class Device {
        PhysicalDevice * _physicalDevice;
        VkDevice _handle;
        std::set<std::string> _enabledExtensions;
        std::vector<std::unique_ptr<QueueFamily>> _queueFamilies;
        std::uint32_t _queueFamilyCount;
        std::vector<std::unique_ptr<ShaderModule>> _shaderCache;
        std::unique_ptr<FencePool> _fencePool;
        std::unique_ptr<SemaphorePool> _semaphorePool;
        std::unique_ptr<DescriptorSetLayoutCache> _descriptorSetLayoutCache;
        std::unique_ptr<PipelineLayoutCache> _pipelineLayoutCache;
        std::unique_ptr<PipelineCache> _pipelineCache;
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
            return _queueFamilies[index].get();
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

        inline std::unique_ptr<Image> createImage(const Image::CreateInfo& info, MemoryUsage memoryUsage) {
            return std::make_unique<Image> (this, info, memoryUsage);
        }

        inline DescriptorSetLayout * allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo) {
            return _descriptorSetLayoutCache->allocateDescriptorSetLayout(createInfo);
        }

        inline PipelineLayout * allocatePipelineLayout(const PipelineLayout::CreateInfo& createInfo) {
            return _pipelineLayoutCache->allocatePipelineLayout(createInfo);
        }

        inline std::unique_ptr<ComputePipeline> createPipeline(const ComputePipeline::CreateInfo& createInfo) {
            return _pipelineCache->createPipeline(createInfo);
        }

        inline std::unique_ptr<GraphicsPipeline> createPipeline(const GraphicsPipeline::CreateInfo& createInfo, const RenderPass * renderPass) {
            return _pipelineCache->createPipeline(createInfo, renderPass);
        }

        inline std::unique_ptr<GraphicsPipeline> createPipeline(const GraphicsPipeline::CreateInfo& createInfo, const std::unique_ptr<RenderPass>& renderPass) {
            return createPipeline(createInfo, renderPass.get());
        }

        inline std::unique_ptr<RenderPass> createRenderPass(const RenderPass::CreateInfo& createInfo) {
            return std::make_unique<RenderPass> (this, createInfo);
        }

        inline std::unique_ptr<Swapchain> createSwapchain(const Swapchain::CreateInfo& createInfo) {
            return std::make_unique<Swapchain> (this, createInfo);
        }

        std::vector<QueueFamily *> getQueueFamilies() const;

        ShaderModule * getShaderModule(const ShaderModule::CreateInfo& createInfo);
    };
}
