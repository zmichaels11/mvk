#pragma once

#include <memory>
#include <queue>
#include <set>

#include "mvk/Fence.hpp"

namespace mvk {
    class Device;

    class FencePool {
        Device * _device;
        std::set<std::unique_ptr<Fence>> _allFences;
        std::queue<Fence * > _availableFences;
        Fence * allocateFence();

    public:
        FencePool(Device * device) :
            _device(device) {}

        FencePool(const FencePool&) = delete;

        FencePool(FencePool&&) = default;

        FencePool& operator=(const FencePool&) = delete;

        FencePool& operator=(FencePool&&) = default;

        inline Device * getDevice() const {
            return _device;
        }

        void releaseFence(Fence * fence);

        Fence * acquireFence();
    };
}
