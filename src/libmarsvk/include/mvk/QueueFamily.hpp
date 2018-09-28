#pragma once

#include <cstdint>

#include "volk.h"

#include <string>

#include "mvk/QueueFlag.hpp"

namespace mvk {
    class Device;

    class QueueFamily {
        std::uint32_t _index;
        const Device * _device;
        VkQueueFamilyProperties _properties;
    
    public:
        QueueFamily(const Device * device, std::uint32_t queueFamilyIndex, const VkQueueFamilyProperties& properties);

        QueueFamily() :
            _index(~0),
            _device(nullptr) {}

        QueueFamily(const QueueFamily&) = delete;

        QueueFamily(QueueFamily&&) = default;

        QueueFamily& operator= (const QueueFamily&) = delete;

        QueueFamily& operator= (QueueFamily&&) = default;

        QueueFlag getFlags() const;

        bool canPresent(VkSurfaceKHR surface) const;

        const Device * getDevice() const;
    };
}
