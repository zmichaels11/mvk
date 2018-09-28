#include "mvk/Semaphore.hpp"

#include "volk.h"

#include "mvk/Device.hpp"
#include "mvk/SemaphorePool.hpp"

namespace mvk {
    Semaphore::~Semaphore() {
        vkDestroySemaphore(getDevice()->getHandle(), _handle, nullptr);
    }

    void Semaphore::release() {
        getSemaphorePool()->releaseSemaphore(this);
    }

    Device * Semaphore::getDevice() const {
        return getSemaphorePool()->getDevice();
    }
}
