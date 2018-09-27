#include "mvk/PhysicalDevice.hpp"

#include <cstdint>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "mvk/MemoryProperties.hpp"

namespace mvk {
    PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle) {
        _handle = handle;

        std::vector<Format> depthStencilFormats;
        depthStencilFormats.push_back(Format::D24_UNORM_S8_UINT);
        depthStencilFormats.push_back(Format::D32_SFLOAT_S8_UINT);
        depthStencilFormats.push_back(Format::D16_UNORM_S8_UINT);

        bool supportsDepthStencil = false;
        for (auto& fmt : depthStencilFormats) {
            VkFormatProperties formatProperties {};

            vkGetPhysicalDeviceFormatProperties(handle, static_cast<VkFormat> (fmt), &formatProperties);

            if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                _preferredDepthStencilFormat = fmt;
                supportsDepthStencil = true;
                break;
            }    
        }

        if (!supportsDepthStencil) {
            throw std::runtime_error("No DepthStencil format is supported!");
        }

        vkGetPhysicalDeviceProperties(handle, &_properties);
        vkGetPhysicalDeviceFeatures(handle, &_features);
        vkGetPhysicalDeviceMemoryProperties(handle, &_memoryProperties);

        _vendor = static_cast<Vendor> (_properties.vendorID);

        auto memoryTypes = _memoryProperties.memoryTypeCount;
        auto umFlags = static_cast<VkMemoryPropertyFlags> (MemoryProperties::UNIFIED);

        bool hasUM = false;
        for (std::uint32_t i = 0; i < memoryTypes; i++) {
            if (_memoryProperties.memoryTypes[i].propertyFlags & umFlags) {
                hasUM = true;
                break;
            }
        }

        _hasUniformMemory = hasUM;
    }
}