#pragma once

#include <memory>
#include <vector>

#include "mvk/Sampler.hpp"

namespace mvk {
    class Device;

    class SamplerCache {
        struct SamplerInstance {
            Sampler instance;
            int references;

            SamplerInstance(SamplerCache * cache, const Sampler::CreateInfo& createInfo) :
                instance(cache, createInfo),
                references(0) {}
        };

        std::vector<std::unique_ptr<SamplerInstance>> _samplers;
        Device * _device;

    public:
        SamplerCache(Device * device) :
            _device(device),
            _samplers() {}

        Sampler * allocate(const Sampler::CreateInfo& createInfo);

        void release(Sampler * sampler);

        inline Device * getDevice() const noexcept {
            return _device;
        }
    };
}
