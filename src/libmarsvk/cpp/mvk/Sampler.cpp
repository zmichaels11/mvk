#include "mvk/Sampler.hpp"

#include "mvk/Device.hpp"
#include "mvk/SamplerCache.hpp"

#include <mvk/Util.hpp>

namespace mvk {
    Device * Sampler::getDevice() const noexcept {
        return _cache->getDevice();
    }

    Sampler::Sampler(SamplerCache * cache, const Sampler::CreateInfo& createInfo) {
        _cache = cache;
        _info = createInfo;

        auto samplerCI = VkSamplerCreateInfo {};
        samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCI.magFilter = static_cast<VkFilter> (createInfo.magFilter);
        samplerCI.minFilter = static_cast<VkFilter> (createInfo.minFilter);
        samplerCI.mipmapMode = static_cast<VkSamplerMipmapMode> (createInfo.mipmapFilter);
        samplerCI.addressModeU = static_cast<VkSamplerAddressMode> (createInfo.addressModeU);
        samplerCI.addressModeV = static_cast<VkSamplerAddressMode> (createInfo.addressModeV);
        samplerCI.addressModeW = static_cast<VkSamplerAddressMode> (createInfo.addressModeW);
        samplerCI.mipLodBias = createInfo.mipLodBias;
        samplerCI.anisotropyEnable = createInfo.anisotropyEnable ? VK_TRUE : VK_FALSE;
        samplerCI.maxAnisotropy = static_cast<VkBool32> (createInfo.maxAnisotropy);
        samplerCI.minLod = createInfo.minLod;
        samplerCI.maxLod = createInfo.maxLod;
        samplerCI.borderColor = static_cast<VkBorderColor> (createInfo.borderColor);
        samplerCI.unnormalizedCoordinates = createInfo.unnormalizedCoordinates ? VK_TRUE : VK_FALSE;

        Util::vkAssert(vkCreateSampler(cache->getDevice()->getHandle(), &samplerCI, nullptr, &_handle));
    }

    Sampler::~Sampler() noexcept {
        vkDestroySampler(getDevice()->getHandle(), _handle, nullptr);
    }

    Sampler& Sampler::operator= (Sampler&& from) noexcept {
        std::swap(this->_cache, from._cache);
        std::swap(this->_handle, from._handle);
        std::swap(this->_info, from._info);

        return *this;
    }

    void Sampler::release() {
        _cache->release(this);
    }
}
