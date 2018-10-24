#pragma once

#include "volk.h"

#include <memory>
#include <utility>

namespace mvk {
    class Device;
    class SemaphorePool;

    class Semaphore {
        VkSemaphore _handle;
        SemaphorePool * _pool;

        Semaphore(const Semaphore&) = delete;
        Semaphore& operator= (const Semaphore&) = delete;

    public:
        std::shared_ptr<void> userPtr;
        
        Semaphore() noexcept:
            _handle(VK_NULL_HANDLE),
            _pool(nullptr) {}

        Semaphore(SemaphorePool * pool, VkSemaphore handle) noexcept:
            _handle(handle),
            _pool(pool) {}

        Semaphore(Semaphore&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _pool(std::move(from._pool)) {}

        ~Semaphore() noexcept;

        Semaphore& operator= (Semaphore&& from) noexcept;

        void release() noexcept;

        inline SemaphorePool * getSemaphorePool() const noexcept {
            return _pool;
        }

        inline VkSemaphore getHandle() const noexcept {
            return _handle;
        }

        Device * getDevice() const noexcept;

        int getFd() const;
    };
}