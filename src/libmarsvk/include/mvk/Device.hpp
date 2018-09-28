#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "mvk/QueueFamily.hpp"
#include "mvk/ShaderModule.hpp"

namespace mvk {
    class PhysicalDevice;

    class Device {
        const PhysicalDevice * _physicalDevice;
        VkDevice _handle;
        std::set<std::string> _enabledExtensions;
        std::unique_ptr<QueueFamily[]> _queueFamilies;
        std::uint32_t _queueFamilyCount;
        std::vector<std::unique_ptr<ShaderModule>> _shaderCache;

    public:
        Device() : 
            _handle(VK_NULL_HANDLE),
            _physicalDevice(nullptr) {}

        Device(const PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions);

        ~Device();

        Device(const Device&) = delete;

        Device(Device&&) = default;

        Device& operator=(const Device&) = delete;

        Device& operator=(Device&&) = default;

        void waitIdle();

        inline const std::set<std::string>& getEnabledExtensions() const noexcept {
            return _enabledExtensions;
        }

        inline const PhysicalDevice * getPhysicalDevice() const noexcept {
            return _physicalDevice;
        }

        inline const QueueFamily * getQueueFamily(std::ptrdiff_t index) const noexcept {
            return _queueFamilies.get() + index;
        }

        inline std::uint32_t getQueueFamilyCount() const noexcept {
            return _queueFamilyCount;
        }

        inline VkDevice getHandle() const {
            return _handle;
        }

        std::vector<const QueueFamily *> getQueueFamilies() const;

        ShaderModule * getShaderModule(const ShaderModule::CreateInfo& createInfo);
    };
}