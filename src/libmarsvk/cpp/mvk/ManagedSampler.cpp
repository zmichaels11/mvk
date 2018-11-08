#include "mvk/ManagedSampler.hpp"

#include "mvk/Sampler.hpp"

namespace mvk {
    ManagedSampler::~ManagedSampler() noexcept {
        if (_pSampler) {
            _pSampler->release();
        }
    }

    ManagedSampler& ManagedSampler::operator= (Sampler * pSampler) noexcept {
        if (_pSampler) {
            _pSampler->release();
        }

        _pSampler = pSampler;
    }

    ManagedSampler& ManagedSampler::operator= (ManagedSampler&& from) noexcept {
        std::swap(this->_pSampler, from._pSampler);

        return *this;
    }
}
