#include "mvk/PhysicalDevice.hpp"

#include <cstdint>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

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
    }

    PhysicalDeviceType PhysicalDevice::getPhysicalDeviceType() const noexcept {
        return static_cast<PhysicalDeviceType> (_properties.deviceType);
    }

    std::string PhysicalDevice::toString() const noexcept {
        auto out = std::stringstream();
        auto version = _properties.apiVersion;
        auto major = VK_VERSION_MAJOR(version);
        auto minor = VK_VERSION_MINOR(version);
        auto patch = VK_VERSION_PATCH(version);

        out << "PhysicalDevice: {DeviceID=0x" << std::hex << _properties.deviceID
            << ", VendorID=0x" << std::hex << _properties.vendorID
            << ", DeviceName=" << _properties.deviceName
            << ", APIVersion=" << std::dec << major << "." << std::dec << minor << "." << std::dec << patch
            << "}";

        return out.str();
    }
}