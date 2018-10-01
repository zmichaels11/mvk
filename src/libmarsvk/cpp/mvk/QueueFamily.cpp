#include "mvk/QueueFamily.hpp"

#include <map>
#include <memory>

#include "mvk/Device.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        thread_local std::map<int, std::unique_ptr<CommandPool>> _commandPools;
    }

    QueueFamily::QueueFamily(Device * device, int queueFamilyIndex, const VkQueueFamilyProperties& properties) {
        _device = device;
        _index = queueFamilyIndex;
        _properties = properties;
    }

    QueueFlag QueueFamily::getFlags() const {
        return static_cast<QueueFlag> (_properties.queueFlags);
    }

    bool QueueFamily::canPresent(VkSurfaceKHR surface) const {
        VkBool32 isSupported = VK_FALSE;

        Util::vkAssert(vkGetPhysicalDeviceSurfaceSupportKHR(_device->getPhysicalDevice()->getHandle(), _index, surface, &isSupported));

        return VK_TRUE == isSupported;
    }

    CommandPool * QueueFamily::getCurrentCommandPool() const {
        auto& pPool = _commandPools[_index];

        if (nullptr == pPool) {
            pPool = std::make_unique<CommandPool> (this, CommandPoolCreateFlag::CREATE_RESET_COMMAND_BUFFER);
        }

        return pPool.get();
    }
}
