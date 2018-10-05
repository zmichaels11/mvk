#include "mvk/QueueFamily.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

#include "mvk/Device.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        constexpr int ALL_QUEUES = -1;
        constexpr int MAX_QUEUES = 1;
    }

    QueueFamily::QueueFamily(Device * device, int queueFamilyIndex, const VkQueueFamilyProperties& properties) {
        _device = device;
        _index = queueFamilyIndex;
        _properties = properties;

        int nQueues = static_cast<int> (properties.queueCount);
        
        if (MAX_QUEUES != ALL_QUEUES) {
            nQueues = std::min(nQueues, MAX_QUEUES);
        }

        _queues.reserve(static_cast<std::size_t> (nQueues));

        for (int i = 0; i < nQueues; i++) {
            _queues.push_back(std::make_unique<Queue> (this, i));
        }

        _commandPool = nullptr;
    }

    QueueFamily::~QueueFamily() {
        _queues.clear();
        detach();
    }

    QueueFlag QueueFamily::getFlags() const {
        return static_cast<QueueFlag> (_properties.queueFlags);
    }

    bool QueueFamily::canPresent(VkSurfaceKHR surface) const {
        VkBool32 isSupported = VK_FALSE;

        Util::vkAssert(vkGetPhysicalDeviceSurfaceSupportKHR(_device->getPhysicalDevice()->getHandle(), _index, surface, &isSupported));

        return VK_TRUE == isSupported;
    }

    CommandPool * QueueFamily::getCurrentCommandPool() {
        if (_commandPool == nullptr) {
            _commandPool = std::make_unique<CommandPool> (this, CommandPoolCreateFlag::CREATE_RESET_COMMAND_BUFFER);
        }
        
        return _commandPool.get();
    }

    void QueueFamily::detach() {
        _commandPool = nullptr;
        //TODO: this should destroy thread_local storage
    }
}
