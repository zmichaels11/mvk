#include "mvk/SamplerCache.hpp"

#include <stdexcept>

#include "mvk/Device.hpp"

namespace mvk {    
    void SamplerCache::release(Sampler * toRemove) {
        auto it = _samplers.begin();
        SamplerInstance * pSampler = nullptr;

        for (; it != _samplers.end(); ++it) {
            if (&it->get()->instance == toRemove) {
                pSampler = it->get();
                break;
            }
        }

        if (nullptr == pSampler) {
            throw std::runtime_error("Failed to release Sampler! Sampler does not belong to SamplerCache!");
        }

        pSampler->references -= 1;

        if (0 == pSampler->references) {
            _samplers.erase(it);
        }
    }

    Sampler * SamplerCache::allocate(const Sampler::CreateInfo& createInfo) {
        auto it = _samplers.begin();
        SamplerInstance * pSampler = nullptr;

        for (; it != _samplers.end(); ++it) {
            if (createInfo == it->get()->instance.getInfo()) {
                pSampler = it->get();
                break;
            }
        }

        if (nullptr == pSampler) {
            auto ptr = std::make_unique<SamplerInstance> (this, createInfo);
            pSampler = ptr.get();

            _samplers.push_back(std::move(ptr));
        }

        pSampler->references += 1;

        return &pSampler->instance;
    }
}
