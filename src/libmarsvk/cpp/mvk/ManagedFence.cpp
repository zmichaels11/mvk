#include "mvk/ManagedFence.hpp"

#include "mvk/Fence.hpp"

namespace mvk {
    ManagedFence::~ManagedFence() noexcept {
        if (_pFence) {
            _pFence->release();
        }
    }

    ManagedFence& ManagedFence::operator= (Fence * pFence) noexcept {
        if (_pFence) {
            _pFence->release();
        }

        _pFence = pFence;

        return *this;
    }

    ManagedFence& ManagedFence::operator= (ManagedFence&& from) noexcept {
        std::swap(this->_pFence, from._pFence);

        return *this;
    }
}
