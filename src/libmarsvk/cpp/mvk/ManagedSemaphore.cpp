#include "mvk/ManagedSemaphore.hpp"

#include "mvk/Semaphore.hpp"

namespace mvk {
    ManagedSemaphore::~ManagedSemaphore() noexcept {
        if (_pSemaphore) {
            _pSemaphore->release();
        }
    }

    ManagedSemaphore& ManagedSemaphore::operator= (Semaphore * pSemaphore) noexcept {
        if (_pSemaphore) {
            _pSemaphore->release();
        }

        _pSemaphore = pSemaphore;

        return *this;
    }

    ManagedSemaphore& ManagedSemaphore::operator= (ManagedSemaphore&& from) noexcept {
        std::swap(this->_pSemaphore, from._pSemaphore);

        return *this;
    }
}
