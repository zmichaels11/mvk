#pragma once

#include <memory>
#include <queue>
#include <set>

#include "mvk/Semaphore.hpp"

namespace mvk {
    class Device;

    class SemaphorePool {
        Device * _device;
        std::set<std::unique_ptr<Semaphore>> _allSemaphores;
        std::queue<Semaphore *> _availableSemaphores;

        Semaphore * allocateSemaphore();

        SemaphorePool(const SemaphorePool&) = delete;
        SemaphorePool& operator= (const SemaphorePool&) = delete;

    public:
        SemaphorePool() noexcept:
            _device(nullptr) {}

        SemaphorePool(Device * device) noexcept:
            _device(device) {}

        SemaphorePool(SemaphorePool&& from) noexcept:
            _device(std::move(from._device)),
            _allSemaphores(std::move(from._allSemaphores)),
            _availableSemaphores(std::move(from._availableSemaphores)) {}

        SemaphorePool& operator= (SemaphorePool&& from) = default;

        inline Device * getDevice() const noexcept {
            return _device;
        }

        Semaphore * acquireSemaphore();

        void releaseSemaphore(Semaphore * semaphore) noexcept;
    };
}