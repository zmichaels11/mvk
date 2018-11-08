#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <memory>
#include <set>
#include <string>
#include <utility>
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
#include "mvk/SamplerCache.hpp"
#include "mvk/SemaphorePool.hpp"
#include "mvk/ShaderModule.hpp"
#include "mvk/Swapchain.hpp"

namespace mvk {
    class PhysicalDevice;

    //! The logical Vulkan Device.
    /*!
        The Device is the direct interface to the current Vulkan context. In addition,
        the Device owns all managed objects and pools.
    */
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
        std::unique_ptr<SamplerCache> _samplerCache;
        VmaAllocator _allocator;

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

    public:
        //! Constructs an empty Device.
        Device() noexcept: 
            _handle(VK_NULL_HANDLE),
            _physicalDevice(nullptr) {}

        Device(Device&& from) noexcept:
            _physicalDevice(std::move(from._physicalDevice)),
            _handle(std::exchange(from._handle, nullptr)),
            _enabledExtensions(std::move(from._enabledExtensions)),
            _queueFamilies(std::move(from._queueFamilies)),
            _queueFamilyCount(std::move(from._queueFamilyCount)),
            _shaderCache(std::move(from._shaderCache)),
            _fencePool(std::move(from._fencePool)),
            _semaphorePool(std::move(from._semaphorePool)),
            _descriptorSetLayoutCache(std::move(from._descriptorSetLayoutCache)),
            _pipelineLayoutCache(std::move(from._pipelineLayoutCache)),
            _pipelineCache(std::move(from._pipelineCache)),
            _samplerCache(std::move(from._samplerCache)),
            _allocator(std::exchange(from._allocator, nullptr)) {}

        //! Constructs a Device object.
        /*!
            \param physicalDevice the Vulkan device to use. Generally this is a GPU.
            \param enabledExtensions is a set of all extensions to enable at Device construction.
        */
        Device(PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions);

        //! Deletes the Device and releases any resources.
        ~Device() noexcept;

        Device& operator= (Device&& from) noexcept;

        //! Busy waits until there are no pending operations for the Device.
        void waitIdle();

        //! Retrieves the set of extensions enabled on construction.
        /*!
            \return the set of enabled extensions.
        */
        inline const std::set<std::string>& getEnabledExtensions() const noexcept {
            return _enabledExtensions;
        }

        //! Retrieves the PhysicalDevice.
        /*!
            \return the PhysicalDevice.
        */
        inline PhysicalDevice * getPhysicalDevice() const noexcept {
            return _physicalDevice;
        }

        //! Retrieves a QueueFamily accessible by the Device.
        /*!
            \param index is the index of the QueueFamily.
            \return the QueueFamily.
        */
        inline QueueFamily * getQueueFamily(std::ptrdiff_t index) const noexcept {
            return _queueFamilies[index].get();
        }

        //! Retrieves the amount of QueueFamilies accessible.
        /*!
            \return the number of QueueFamilies.
        */
        inline std::size_t getQueueFamilyCount() const noexcept {
            return _queueFamilyCount;
        }

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the handle.
        */
        inline VkDevice getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkDevice() const noexcept {
            return _handle;
        }

        //! Acquires a Fence from the FencePool.
        /*!
            \return the Fence.
        */
        inline Fence * acquireFence() {
            return _fencePool->acquireFence();
        }

        //! Acquires a Semaphore from the SemaphorePool.
        /*!
            \return the Semaphore.
        */
        inline Semaphore * acquireSemaphore() {
            return _semaphorePool->acquireSemaphore();
        }

        //! Retrieves the memory allocator.
        /*!
            /return the memory allocator.
        */
        inline VmaAllocator& getMemoryAllocator() noexcept {
            return _allocator;
        }

        //! Constructs a new Buffer object.
        /*!
            \param info is the construction parameters.
            \param memoryUsage is the preferred usage of the accompanying memory object.
            \return the new Buffer wrapped in a unique_ptr.
        */
        inline UPtrBuffer createBuffer(const Buffer::CreateInfo& info, MemoryUsage memoryUsage) {
            return std::make_unique<Buffer> (this, info, memoryUsage);
        }

        //! Constructs a new Image object.
        /*!
            \param info is the construction parameters.
            \param memoryUsage is the preferred usage of the accompanying memory object.
            \return the new Image wrapped in a unique_ptr.
        */
        inline UPtrImage createImage(const Image::CreateInfo& info, MemoryUsage memoryUsage) {
            return std::make_unique<Image> (this, info, memoryUsage);
        }

        //! Allocates a new DescriptorSetLayout
        /*!
            Allocates or reuses a DescriptorSetLayout from the DescriptorSetLayoutCache.

            \param createInfo is the construction parameters.
            \return the DescriptorSetLayout.
        */
        inline DescriptorSetLayout * allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo) {
            return _descriptorSetLayoutCache->allocateDescriptorSetLayout(createInfo);
        }

        //! Allocates a new PipelineLayout.
        /*!
            Allocates or reuses a PipelineLayout from the PipelineLayoutCache.

            /param createInfo is the construction parameters.
            /return the PipelineLayout
        */
        inline PipelineLayout * allocatePipelineLayout(const PipelineLayout::CreateInfo& createInfo) {
            return _pipelineLayoutCache->allocatePipelineLayout(createInfo);
        }

        //! Creates a new ComputePipeline.
        /*!
            \param createInfo is the construction parameters.
            \return the ComputePipeline wrapped in a unique_ptr.
        */
        inline UPtrComputePipeline createPipeline(const ComputePipeline::CreateInfo& createInfo) {
            return _pipelineCache->createPipeline(createInfo);
        }

        //! Creates a new GraphicsPipeline.
        /*!
            \param createInfo is the construction parameters.
            \param renderPass is a pointer to the RenderPass used by the pipeline.
            \return the GraphicsPipeline wrapped in a unique_ptr.
        */
        inline UPtrGraphicsPipeline createPipeline(const GraphicsPipeline::CreateInfo& createInfo, const RenderPass * renderPass) {
            return _pipelineCache->createPipeline(createInfo, renderPass);
        }

        //! Creates a new GraphicsPipeline.
        /*!
            This function unwraps the unique_ptr for the RenderPass and chains the raw pointer variant.
        */
        inline UPtrGraphicsPipeline createPipeline(const GraphicsPipeline::CreateInfo& createInfo, const std::unique_ptr<RenderPass>& renderPass) {
            return createPipeline(createInfo, renderPass.get());
        }

        //! Creates a new RenderPass.
        /*!
            \param createInfo is the construction parameters.
            \return the new RenderPass wrapped in a unique_ptr.
        */
        inline UPtrRenderPass createRenderPass(const RenderPass::CreateInfo& createInfo) {
            return std::make_unique<RenderPass> (this, createInfo);
        }

        //! Creates a new SwapChain.
        /*!
            \param createInfo is the construction parameters.
            \return the new Swapchain wrapped in a unique_ptr.
        */
        inline UPtrSwapchain createSwapchain(const Swapchain::CreateInfo& createInfo) {
            return std::make_unique<Swapchain> (this, createInfo);
        }

        //! Allocates a new Sampler.
        /*!
            Allocates or reuses a new Sampler from the SamplerCache.
            
            \param createInfo is the construction parameters.
            \return the Sampler.
        */
        inline Sampler * allocateSampler(const Sampler::CreateInfo& createInfo) {
            return _samplerCache->allocate(createInfo);
        }

        //! Retrieves a list of all supported QueueFamily objects.
        /*!
            \return vector of supported QueueFamilies.
        */
        std::vector<QueueFamily *> getQueueFamilies() const noexcept;

        //! Retrieves a ShaderModule.
        /*!
            \param createInfo is the ShaderModule construction parameters.
            \return the ShaderModule.
        */
        ShaderModule * getShaderModule(const ShaderModule::CreateInfo& createInfo);
    };
}
