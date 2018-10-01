#pragma once

#include <cstdint>

#include "volk.h"

#include <string>

#include "mvk/CommandPool.hpp"
#include "mvk/QueueFlag.hpp"

namespace mvk {
    class Device;

    class QueueFamily {
        int _index;
        Device * _device;
        VkQueueFamilyProperties _properties;
    
    public:
        QueueFamily(Device * device, int queueFamilyIndex, const VkQueueFamilyProperties& properties);

        QueueFamily() :
            _index(-1),
            _device(nullptr) {}

        QueueFamily(const QueueFamily&) = delete;

        QueueFamily(QueueFamily&&) = default;

        QueueFamily& operator= (const QueueFamily&) = delete;

        QueueFamily& operator= (QueueFamily&&) = default;

        QueueFlag getFlags() const;

        CommandPool * getCurrentCommandPool() const;

        bool canPresent(VkSurfaceKHR surface) const;

        inline Device * getDevice() const {
            return _device;
        }

        inline int getIndex() const {
            return _index;
        }

        inline const VkQueueFamilyProperties& getProperties() const {
            return _properties;
        }
    };
}
