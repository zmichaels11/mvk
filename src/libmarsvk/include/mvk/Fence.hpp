#pragma once

#include "volk.h"

namespace mvk {
    class Device;
    class FencePool;

    class Fence {
        VkFence _handle;
        FencePool * _pool;

    public:
        Fence() :
            _handle(nullptr),
            _pool(nullptr) {}

        Fence(FencePool * pool, VkFence handle) :
            _handle(handle),
            _pool(pool) {}

        ~Fence();

        Fence(const Fence&) = delete;

        Fence(Fence&&) = default;

        Fence& operator= (const Fence&) = delete;

        Fence& operator= (Fence&&) = default;

        inline const VkFence getHandle() const {
            return _handle;
        }

        inline FencePool * getFencePool() const {
            return _pool;
        }

        Device * getDevice() const;

        bool isSignaled() const;

        void reset();

        void waitFor();

        void release();
    };
}