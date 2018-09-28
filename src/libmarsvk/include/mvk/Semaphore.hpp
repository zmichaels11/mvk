#pragma once

#include "volk.h"

namespace mvk {
    class Device;
    class SemaphorePool;

    class Semaphore {
        VkSemaphore _handle;
        SemaphorePool * _pool;

    public:
        Semaphore(SemaphorePool * pool, VkSemaphore handle) :
            _handle(handle),
            _pool(pool) {}

        Semaphore(const Semaphore&) = delete;

        Semaphore(Semaphore&&) = default;

        ~Semaphore();

        Semaphore& operator= (const Semaphore&) = delete;

        Semaphore& operator= (Semaphore&&) = default;

        void release();

        inline SemaphorePool * getSemaphorePool() const {
            return _pool;
        }

        inline VkSemaphore getHandle() const {
            return _handle;
        }

        Device * getDevice() const;
    };
}