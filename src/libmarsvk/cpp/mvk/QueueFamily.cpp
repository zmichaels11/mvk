#include "mvk/QueueFamily.hpp"

#include "mvk/Device.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    QueueFamily::QueueFamily(Device * device, std::uint32_t queueFamilyIndex, const VkQueueFamilyProperties& properties) {
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
}
