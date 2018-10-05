#pragma once

#include "volk.h"

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

    public:
        Sampler(SamplerCache * cache, const CreateInfo& createInfo);

        ~Sampler();

        Sampler& operator= (const Sampler&) = delete;

        Sampler& operator= (Sampler&&) = default;

        inline operator VkSampler() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline SamplerCache * getSamplerCache() const noexcept {
            return _cache;
        }

        Device * getDevice() const;

        void release();
    };

    inline constexpr bool operator== (const Sampler::CreateInfo& lhs, const Sampler::CreateInfo& rhs) {
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
