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

    public:
        SemaphorePool(Device * device) :
            _device(device) {}

        inline Device * getDevice() const {
            return _device;
        }

        Semaphore * acquireSemaphore();

        void releaseSemaphore(Semaphore * semaphore);
    };
}