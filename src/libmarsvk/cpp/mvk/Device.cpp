#include "mvk/Device.hpp"

#include <cstdint>

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

        _queueFamilies = std::make_unique<QueueFamily[]> (_queueFamilyCount);

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            auto qf = QueueFamily(this, i, pQueueFamilyProperties[i]);

            std::swap(qf, _queueFamilies[i]);
        }

        VmaAllocatorCreateInfo vmaAllocatorCI {};

        vmaAllocatorCI.physicalDevice = physicalDevice->getHandle();
        vmaAllocatorCI.device = _handle;

        vmaCreateAllocator(&vmaAllocatorCI, &_allocator);

        _fencePool = std::make_unique<FencePool> (this);
        _semaphorePool = std::make_unique<SemaphorePool> (this);
    }

    Device::~Device() {
        waitIdle();

        _fencePool.reset();
        _semaphorePool.reset();
        _shaderCache.clear();

        vmaDestroyAllocator(_allocator);

        vkDestroyDevice(_handle, nullptr);
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

    std::vector<QueueFamily * > Device::getQueueFamilies() const {
        auto out = std::vector<QueueFamily *>();

        out.reserve(_queueFamilyCount);

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            out.push_back(_queueFamilies.get() + i);
        }

        return out;
    }
}
