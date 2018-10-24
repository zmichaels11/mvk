#pragma once

#include "volk.h"

#include <utility>

#include "mvk/BorderColor.hpp"
#include "mvk/CompareOp.hpp"
#include "mvk/Filter.hpp"
#include "mvk/SamplerAddressMode.hpp"
#include "mvk/SamplerMipmapFilter.hpp"

namespace mvk {
    class Device;
    class SamplerCache;

    class Sampler {
    public:
        struct CreateInfo {
            unsigned int flags;
            Filter minFilter;
            Filter magFilter;
            SamplerMipmapFilter mipmapFilter;
            SamplerAddressMode addressModeU;
            SamplerAddressMode addressModeV;
            SamplerAddressMode addressModeW;
            float mipLodBias;
            bool anisotropyEnable;
            float maxAnisotropy;
            bool compareEnable;
            CompareOp compareOp;
            float minLod;
            float maxLod;
            BorderColor borderColor;
            bool unnormalizedCoordinates;
        };

    private:
        VkSampler _handle;
        SamplerCache* _cache;
        CreateInfo _info;

        Sampler(const Sampler&) = delete;
        Sampler& operator= (const Sampler&) = delete;

    public:
        Sampler() noexcept:
            _handle(VK_NULL_HANDLE),
            _cache(nullptr) {}

        Sampler(SamplerCache * cache, const CreateInfo& createInfo);

        Sampler(Sampler&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _cache(std::move(from._cache)),
            _info(std::move(from._info)) {}

        ~Sampler() noexcept;

        Sampler& operator= (Sampler&& from) noexcept;

        inline operator VkSampler() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline SamplerCache * getSamplerCache() const noexcept {
            return _cache;
        }

        inline VkSampler getHandle() const noexcept {
            return _handle;
        }

        Device * getDevice() const noexcept;

        void release();
    };

    inline constexpr bool operator== (const Sampler::CreateInfo& lhs, const Sampler::CreateInfo& rhs) noexcept {
        return lhs.flags == rhs.flags
            && lhs.minFilter == rhs.minFilter
            && lhs.magFilter == rhs.magFilter
            && lhs.mipmapFilter == rhs.mipmapFilter
            && lhs.addressModeU == rhs.addressModeU
            && lhs.addressModeV == rhs.addressModeV
            && lhs.addressModeW == rhs.addressModeW
            && lhs.compareOp == rhs.compareOp
            && lhs.borderColor == rhs.borderColor
            && lhs.unnormalizedCoordinates == rhs.unnormalizedCoordinates
            && lhs.mipLodBias == rhs.mipLodBias
            && lhs.minLod == rhs.minLod
            && lhs.maxLod == rhs.maxLod;
    }
}
