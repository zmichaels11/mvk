#include "mvk/Device.hpp"

#include <cstdint>

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    void Device::waitIdle() {
        Util::vkAssert(vkDeviceWaitIdle(_handle));
    }

    Device::Device(PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions) {
        _physicalDevice = physicalDevice;
        _enabledExtensions = enabledExtensions;

        auto pdHandle = physicalDevice->getHandle();

        vkGetPhysicalDeviceQueueFamilyProperties(pdHandle, &_queueFamilyCount, nullptr);

        auto pQueueFamilyProperties = std::make_unique<VkQueueFamilyProperties[]> (_queueFamilyCount);

        vkGetPhysicalDeviceQueueFamilyProperties(pdHandle, &_queueFamilyCount, pQueueFamilyProperties.get());

        auto pDeviceQueueCI = std::vector<VkDeviceQueueCreateInfo>();

        pDeviceQueueCI.reserve(_queueFamilyCount);

        float priorities = 1.0F;

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            VkDeviceQueueCreateInfo deviceQueueCI {};

            deviceQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCI.pQueuePriorities = &priorities;
            deviceQueueCI.queueCount = 1;
            deviceQueueCI.queueFamilyIndex = i;

            pDeviceQueueCI.push_back(deviceQueueCI);
        }

        auto pEnabledExtensions = std::vector<const char *>();

        pEnabledExtensions.reserve(enabledExtensions.size());

        for (auto& extName : enabledExtensions) {
            pEnabledExtensions.push_back(extName.c_str());
        }

        VkDeviceCreateInfo deviceCI {};

        deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCI.pQueueCreateInfos = pDeviceQueueCI.data();
        deviceCI.queueCreateInfoCount = pDeviceQueueCI.size();
        deviceCI.ppEnabledExtensionNames = pEnabledExtensions.data();
        deviceCI.enabledExtensionCount = pEnabledExtensions.size();

        Util::vkAssert(vkCreateDevice(pdHandle, &deviceCI, nullptr, &_handle));

        _queueFamilies.reserve(_queueFamilyCount);

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            _queueFamilies.push_back(std::make_unique<QueueFamily>(this, i, pQueueFamilyProperties[i]));
        }

        auto vmaVulkanFunctions = VmaVulkanFunctions {};
        vmaVulkanFunctions.vkAllocateMemory = vkAllocateMemory;
        vmaVulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
        vmaVulkanFunctions.vkBindImageMemory = vkBindImageMemory;
        vmaVulkanFunctions.vkCreateBuffer = vkCreateBuffer;
        vmaVulkanFunctions.vkCreateImage = vkCreateImage;
        vmaVulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
        vmaVulkanFunctions.vkDestroyImage = vkDestroyImage;
        vmaVulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
        vmaVulkanFunctions.vkFreeMemory = vkFreeMemory;
        vmaVulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
        vmaVulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2;
        vmaVulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
        vmaVulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2;
        vmaVulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
        vmaVulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
        vmaVulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
        vmaVulkanFunctions.vkMapMemory = vkMapMemory;
        vmaVulkanFunctions.vkUnmapMemory = vkUnmapMemory;

        auto vmaAllocatorCI = VmaAllocatorCreateInfo {};

        vmaAllocatorCI.pVulkanFunctions = &vmaVulkanFunctions;
        vmaAllocatorCI.physicalDevice = physicalDevice->getHandle();
        vmaAllocatorCI.device = _handle;
    
        _allocator = VK_NULL_HANDLE;        

        vmaCreateAllocator(&vmaAllocatorCI, &_allocator);

        _fencePool = std::make_unique<FencePool> (this);
        _semaphorePool = std::make_unique<SemaphorePool> (this);
        _descriptorSetLayoutCache = std::make_unique<DescriptorSetLayoutCache> (this);
        _pipelineLayoutCache = std::make_unique<PipelineLayoutCache> (this);
        _pipelineCache = std::make_unique<PipelineCache> (this);
        _samplerCache = std::make_unique<SamplerCache> (this);
    }

    Device::~Device() noexcept {
        try {
            waitIdle();
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }
        
        _samplerCache = nullptr;
        _pipelineLayoutCache = nullptr;
        _pipelineCache = nullptr;
        _descriptorSetLayoutCache = nullptr;
        _semaphorePool = nullptr;
        _fencePool = nullptr;
        _shaderCache.clear();
        _queueFamilies.clear();

        vmaDestroyAllocator(_allocator);

        vkDestroyDevice(_handle, nullptr);
    }

    Device& Device::operator= (Device&& from) noexcept {
        std::swap(this->_allocator, from._allocator);
        std::swap(this->_descriptorSetLayoutCache, from._descriptorSetLayoutCache);
        std::swap(this->_enabledExtensions, from._enabledExtensions);
        std::swap(this->_fencePool, from._fencePool);
        std::swap(this->_handle, from._handle);
        std::swap(this->_physicalDevice, from._physicalDevice);
        std::swap(this->_pipelineCache, from._pipelineCache);
        std::swap(this->_pipelineLayoutCache, from._pipelineLayoutCache);
        std::swap(this->_queueFamilies, from._queueFamilies);
        std::swap(this->_queueFamilyCount, from._queueFamilyCount);
        std::swap(this->_samplerCache, from._samplerCache);
        std::swap(this->_semaphorePool, from._semaphorePool);

        return *this;
    }

    ShaderModule * Device::getShaderModule(const ShaderModule::CreateInfo& createInfo) {
        for (const auto& module : _shaderCache) {
            if (module->getInfo() == createInfo) {
                return module.get();
            }
        }

        auto ptr = std::make_unique<ShaderModule> (this, createInfo);
        auto out = ptr.get();

        _shaderCache.push_back(std::move(ptr));

        return out;
    }

    std::vector<QueueFamily * > Device::getQueueFamilies() const noexcept {
        auto out = std::vector<QueueFamily *>();

        out.reserve(_queueFamilyCount);

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            out.push_back(_queueFamilies[i].get());
        }

        return out;
    }
}
