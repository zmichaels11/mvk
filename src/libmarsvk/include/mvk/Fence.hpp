#pragma once

#include "volk.h"

#include <memory>
#include <utility>

namespace mvk {
    class Device;
    class FencePool;

    //! A Fence object represents a Device-to-Host queryable synchronization object.
    class Fence {
        VkFence _handle;
        FencePool * _pool;

        Fence(const Fence&) = delete;
        Fence& operator= (const Fence&) = delete;

    public:
        std::shared_ptr<void> userPtr;

        //! Constructs an empty Fence object.
        Fence() noexcept:
            _handle(nullptr),
            _pool(nullptr) {}

        //! Constructs a Fence object by wrapping Vulkan handle.
        /*!
            \param pool is the FencePool that will own the Fence.
            \param handle is the Vulkan handle to wrap.
        */
        Fence(FencePool * pool, VkFence handle) noexcept:
            _handle(handle),
            _pool(pool) {}

        //! Deletes the Fence object and releases all resources.
        ~Fence() noexcept;

        //! Move-constructs the Fence.
        /*!
            \param from the other Fence.
        */
        Fence(Fence&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _pool(std::move(from._pool)) {}

        //! Move-assigns the Fence.
        /*!
            \param from the other Fence.
        */
        Fence& operator= (Fence&& from) noexcept;

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Fence.
        */
        inline VkFence getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan handle.
        /*!
            \return the underlying handle.
        */
        inline operator VkFence() const noexcept {
            return _handle;
        }

        //! Retrieves the FencePool that owns this Fence.
        /*!
            \return the FencePool.
        */
        inline FencePool * getFencePool() const noexcept {
            return _pool;
        }

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Checks if the Fence was signaled.
        /*!
            \return true if the Fence was signaled.
        */
        bool isSignaled() const;

        //! Clears the signal state of the Fence.
        void reset();

        //! Busy waits until the Fence is signaled.
        void waitFor();

        //! Returns the Fence to the FencePool.
        void release() noexcept;
    };
}
