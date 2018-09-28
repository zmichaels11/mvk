#pragma once

#include <cstdint>

#include "volk.h"

#include <string>

#include "mvk/QueueFlag.hpp"

namespace mvk {
    class Device;

    class QueueFamily {
        std::uint32_t _index;
        Device * _device;
        VkQueueFamilyProperties _properties;
    
    public:
        QueueFamily(Device * device, std::uint32_t queueFamilyIndex, const VkQueueFamilyProperties& properties);

        QueueFamily() :
            _index(~0),
            _device(nullptr) {}

        QueueFamily(const QueueFamily&) = delete;

        QueueFamily(QueueFamily&&) = default;

        QueueFamily& operator= (const QueueFamily&) = delete;

        QueueFamily& operator= (QueueFamily&&) = default;

        QueueFlag getFlags() const;

        bool canPresent(VkSurfaceKHR surface) const;

        inline Device * getDevice() const {
            return _device;
        }

        inline std::uint32_t getIndex() const {
            return _index;
        }

        inline const VkQueueFamilyProperties& getProperties() const {
            return _properties;
        }
    };
}
