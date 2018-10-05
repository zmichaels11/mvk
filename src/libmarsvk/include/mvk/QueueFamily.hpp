#pragma once

#include <cstdint>

#include "volk.h"

#include <memory>
#include <string>
#include <vector>

#include "mvk/CommandPool.hpp"
#include "mvk/Queue.hpp"
#include "mvk/QueueFlag.hpp"

namespace mvk {
    class Device;

    class QueueFamily {
        int _index;
        Device * _device;
        VkQueueFamilyProperties _properties;
        std::vector<std::unique_ptr<Queue>> _queues;

        //TODO: this should be thread_local
        std::unique_ptr<CommandPool> _commandPool;
    
    public:
        QueueFamily(Device * device, int queueFamilyIndex, const VkQueueFamilyProperties& properties);

        QueueFamily() :
            _index(-1),
            _device(nullptr) {}

        QueueFamily(const QueueFamily&) = delete;

        QueueFamily(QueueFamily&&) = default;

        ~QueueFamily();

        QueueFamily& operator= (const QueueFamily&) = delete;

        QueueFamily& operator= (QueueFamily&&) = default;

        QueueFlag getFlags() const;

        CommandPool * getCurrentCommandPool();

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

        inline Queue * getQueue(int queueIndex) const noexcept {
            return _queues[queueIndex].get();
        }

        void detach();
    };
}
