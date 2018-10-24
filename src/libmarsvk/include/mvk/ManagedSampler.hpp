#pragma once

#include <utility>

namespace mvk {
    class Sampler;
    
    //! ManagedSampler is a wrapper for the Sampler object that will release to the SamplerPool when it exits scope.
    class ManagedSampler {
        Sampler * _pSampler;

        ManagedSampler(const ManagedSampler&) = delete;
        ManagedSampler& operator= (const ManagedSampler&) = delete;

    public:
        //! Constructs an empty ManagedSampler.
        ManagedSampler() noexcept:
            _pSampler(nullptr) {}

        //! Constructs a ManagedSampler by wrapping a pool-allocated Sampler.
        /*!
            \param pSampler is the pointer to a Sampler allocated from a SamplerPool.
        */
        ManagedSampler(Sampler * pSampler) noexcept:
            _pSampler(pSampler) {}

        //! Deconstructs the ManagedSampler and releases any resources back to the SamplerPool.
        ~ManagedSampler() noexcept;

        //! Move-constructs the ManagedSampler.
        ManagedSampler(ManagedSampler&& from) noexcept:
            _pSampler(std::exchange(from._pSampler, nullptr)) {}

        ManagedSampler& operator= (ManagedSampler&&) noexcept;
        
        ManagedSampler& operator= (Sampler * pSampler) noexcept;

        inline Sampler * get() const noexcept {
            return _pSampler;
        }

        inline Sampler * operator->() const noexcept {
            return _pSampler;
        }

        inline operator Sampler*() const noexcept {
            return _pSampler;
        }
    };
}
