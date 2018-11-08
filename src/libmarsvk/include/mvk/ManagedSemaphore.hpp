#pragma once

#include <utility>

namespace mvk {
    class Semaphore;

    class ManagedSemaphore {
        Semaphore * _pSemaphore;

        ManagedSemaphore(const ManagedSemaphore&) = delete;
        ManagedSemaphore& operator= (const ManagedSemaphore&) = delete;

    public:
        ManagedSemaphore() noexcept:
            _pSemaphore(nullptr) {}
            
        ManagedSemaphore(Semaphore * pSemaphore) noexcept:
            _pSemaphore(pSemaphore) {}

        ~ManagedSemaphore() noexcept;

        ManagedSemaphore(ManagedSemaphore&& from) noexcept:
            _pSemaphore(std::exchange(from._pSemaphore, nullptr)) {}

        ManagedSemaphore& operator= (ManagedSemaphore&&) noexcept;

        ManagedSemaphore& operator= (Semaphore * pSemaphore) noexcept;

        inline Semaphore * get() const noexcept {
            return _pSemaphore;
        }

        inline Semaphore * operator->() const noexcept {
            return _pSemaphore;
        }

        inline operator Semaphore*() const noexcept {
            return _pSemaphore;
        }
    };
}
