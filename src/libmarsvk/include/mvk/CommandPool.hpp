#pragma once

#include "volk.h"

#include <memory>

#include "mvk/CommandBuffer.hpp"
#include "mvk/CommandBufferLevel.hpp"
#include "mvk/CommandPoolCreateFlag.hpp"

namespace mvk {
    class Device;
    class QueueFamily;

    class CommandPool {
        VkCommandPool _handle;
        const QueueFamily * _queueFamily;
        CommandPoolCreateFlag _flags;

    public:
        CommandPool(const QueueFamily * queueFamily, CommandPoolCreateFlag flags);

        CommandPool():
            _handle(VK_NULL_HANDLE),
            _queueFamily(nullptr) {}

        CommandPool(const CommandPool&) = delete;

        CommandPool(CommandPool&&) = default;

        ~CommandPool();

        CommandPool& operator= (const CommandPool&) = delete;

        CommandPool& operator= (CommandPool&&) = default;

        Device * getDevice() const;

        inline const QueueFamily * getQueueFamily() const noexcept {
            return _queueFamily;
        }

        inline VkCommandPool getHandle() const noexcept {
            return _handle;
        }

        inline CommandPoolCreateFlag getFlags() const noexcept {
            return _flags;
        }

        void reset(unsigned int flags = 0);

        std::unique_ptr<CommandBuffer> allocate(CommandBufferLevel level = CommandBufferLevel::PRIMARY);        
    };
}
