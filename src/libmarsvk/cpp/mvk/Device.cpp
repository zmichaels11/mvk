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

    Device::Device(const PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions) {
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
    }

    Device::~Device() {
        waitIdle();

        vkDestroyDevice(_handle, nullptr);
    }

    std::vector<const QueueFamily * > Device::getQueueFamilies() const {
        auto out = std::vector<const QueueFamily *>();

        out.reserve(_queueFamilyCount);

        for (std::uint32_t i = 0; i < _queueFamilyCount; i++) {
            out.push_back(_queueFamilies.get() + i);
        }

        return out;
    }
}