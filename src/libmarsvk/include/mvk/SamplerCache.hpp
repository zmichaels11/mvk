#pragma once

#include <memory>
#include <utility>
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

        Device * _device;
        std::vector<std::unique_ptr<SamplerInstance>> _samplers;

        SamplerCache(const SamplerCache&) = delete;
        SamplerCache& operator= (const SamplerCache&) = delete;

    public:
        SamplerCache() noexcept:
            _device(nullptr) {}

        SamplerCache(Device * device) noexcept:
            _device(device),
            _samplers() {}

        SamplerCache(SamplerCache&& from) noexcept:
            _device(std::move(from._device)),
            _samplers(std::move(from._samplers)) {}

        SamplerCache& operator= (SamplerCache&& from) = default;

        Sampler * allocate(const Sampler::CreateInfo& createInfo);

        void release(Sampler * sampler);

        inline Device * getDevice() const noexcept {
            return _device;
        }
    };
}
