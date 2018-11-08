#include "mvk/Semaphore.hpp"

#include "volk.h"

#include "mvk/Device.hpp"
#include "mvk/SemaphorePool.hpp"

namespace mvk {
    Semaphore::~Semaphore() noexcept {
        vkDestroySemaphore(getDevice()->getHandle(), _handle, nullptr);
    }

    Semaphore& Semaphore::operator= (Semaphore&& from) noexcept {
        std::swap(this->_handle, from._handle);
        std::swap(this->_pool, from._pool);

        return *this;
    }

    void Semaphore::release() noexcept {
        getSemaphorePool()->releaseSemaphore(this);
    }

    Device * Semaphore::getDevice() const noexcept {
        return getSemaphorePool()->getDevice();
    }

    int Semaphore::getFd() const {
        auto semaphoreGetFdInfo = VkSemaphoreGetFdInfoKHR {};
        semaphoreGetFdInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR;
        semaphoreGetFdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT;
        semaphoreGetFdInfo.semaphore = _handle;

        int fd;

        vkGetSemaphoreFdKHR(getDevice()->getHandle(), &semaphoreGetFdInfo, &fd);

        return fd;
    }
}
