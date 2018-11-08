#pragma once

#include "volk.h"

#include <memory>
#include <utility>

#include "mvk/CommandBuffer.hpp"
#include "mvk/CommandBufferLevel.hpp"
#include "mvk/CommandPoolCreateFlag.hpp"

namespace mvk {
    class Device;
    class QueueFamily;

    //! A pool resource that manages and allocates CommandBuffer objects.
    /*!
        Command pools are opaque objects that command buffer memory is allocated from, 
        and which allow the implementation to amortize the cost of resource creation across 
        multiple command buffers. Command pools are externally synchronized, meaning that a 
        command pool must not be used concurrently in multiple threads. That includes use via 
        recording commands on any command buffers allocated from the pool, as well as operations 
        that allocate, free, and reset command buffers or the pool itself.

        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCommandPool.html">VkCommandPool</a>
    */
    class CommandPool {
        VkCommandPool _handle;
        const QueueFamily * _queueFamily;
        CommandPoolCreateFlag _flags;

        CommandPool(const CommandPool&) = delete;
        CommandPool& operator= (const CommandPool&) = delete;

    public:
        //! Constructs a new CommandPool
        /*!
            \param queueFamily the QueueFamily.
            \param flags bitmask indicating usage behavior for the pool and CommandBuffers allocated from it.
        */
        CommandPool(const QueueFamily * queueFamily, CommandPoolCreateFlag flags);

        //! Constructs a CommandPool wrapping nothing.
        CommandPool() noexcept:
            _handle(VK_NULL_HANDLE),
            _queueFamily(nullptr) {}

        //! Move-constructs the CommandPool.
        /*!
            \param from the other CommandPool.
        */
        CommandPool(CommandPool&& from) noexcept: 
            _handle(std::exchange(from._handle, nullptr)),
            _queueFamily(std::move(from._queueFamily)),
            _flags(std::move(from._flags)) {}

        //! Deletes the CommandPool and releases all resources.
        ~CommandPool() noexcept;
        
        //! Move-assigns the CommandPool.
        /*!
            \param from the other CommandPool.
        */
        CommandPool& operator= (CommandPool&& from) noexcept;

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Retrieves the QueueFamily.
        /*!
            \return the QueueFamily.
        */
        inline const QueueFamily * getQueueFamily() const noexcept {
            return _queueFamily;
        }

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Vulkan handle.
        */
        inline VkCommandPool getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan CommandPool handle.
        inline operator VkCommandPool() const noexcept {
            return _handle;
        }

        //! Retrieves the flags used in construction of the CommandPool.
        /*!
            \return the usage behavior flags of both the CommandPool and CommandBuffers allocated by this CommandPool.
        */
        inline CommandPoolCreateFlag getFlags() const noexcept {
            return _flags;
        }

        //! Resets the CommandPool.
        /*!
            Resetting a CommandPool recycles all of the resources from all of the CommandBuffers allocated from the CommandPool
            back to the CommandPool. All CommandBuffers that have been allocated from the CommandPool are put in the initial state.

            Any primary CommandBuffers allocated from another CommandPool that is in the recording or executable state and has 
            a secondary CommandBuffer allocated from CommandPool recorded into it, becomes invalid.

            See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkResetCommandPool.html">vkResetCommandPool</a>

            \param flags Bitmask controlling the behavior of the CommandPool reset.
        */
        void reset(unsigned int flags = 0);

        //! Allocates a CommandBuffer
        /*!
            See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkAllocateCommandBuffers.html">vkAllocateCommandBuffers</a>

            \param level is the level of the CommandBuffer.
            \return unique_ptr pointing to the newly allocated CommandBuffer.
        */
        UPtrCommandBuffer allocate(CommandBufferLevel level = CommandBufferLevel::PRIMARY);        
    };
}
