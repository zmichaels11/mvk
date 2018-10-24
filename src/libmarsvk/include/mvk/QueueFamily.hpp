#pragma once

#include <cstddef>

#include "volk.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mvk/CommandPool.hpp"
#include "mvk/Queue.hpp"
#include "mvk/QueueFlag.hpp"

namespace mvk {
    class Device;
    class Surface;

    class QueueFamily {
        int _index;
        Device * _device;
        VkQueueFamilyProperties _properties;
        std::vector<std::unique_ptr<Queue>> _queues;

        //TODO: this should be thread_local
        std::unique_ptr<CommandPool> _commandPool;

        QueueFamily(const QueueFamily&) = delete;
        QueueFamily& operator= (const QueueFamily&) = delete;
    
    public:
        QueueFamily(Device * device, int queueFamilyIndex, const VkQueueFamilyProperties& properties) noexcept;

        QueueFamily() noexcept:
            _index(-1),
            _device(nullptr) {}

        QueueFamily(QueueFamily&& from) noexcept:
            _index(std::move(from._index)),
            _device(std::move(from._device)),
            _properties(std::move(from._properties)),
            _queues(std::move(from._queues)) {}

        ~QueueFamily() noexcept;

        QueueFamily& operator= (QueueFamily&&) = default;

        QueueFlag getFlags() const noexcept;

        CommandPool * getCurrentCommandPool();

        bool canPresent(const Surface * surface) const;

        inline bool canPresent(const std::unique_ptr<Surface>& surface) const {
            return canPresent(surface.get());
        }

        inline Device * getDevice() const noexcept {
            return _device;
        }

        inline int getIndex() const noexcept {
            return _index;
        }

        inline const VkQueueFamilyProperties& getProperties() const noexcept {
            return _properties;
        }

        inline Queue * getQueue(std::ptrdiff_t queueIndex) const noexcept {
            return _queues[queueIndex].get();
        }

        inline std::size_t getQueeuCount() const noexcept {
            return _queues.size();
        }

        void detach() noexcept;
    };
}
