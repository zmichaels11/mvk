#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "mvk/FencePool.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/ShaderModule.hpp"

namespace mvk {
    class PhysicalDevice;

    class Device {
        PhysicalDevice * _physicalDevice;
        VkDevice _handle;
        std::set<std::string> _enabledExtensions;
        std::unique_ptr<QueueFamily[]> _queueFamilies;
        std::uint32_t _queueFamilyCount;
        std::vector<std::unique_ptr<ShaderModule>> _shaderCache;
        std::unique_ptr<FencePool> _fencePool;

    public:
        Device() : 
            _handle(VK_NULL_HANDLE),
            _physicalDevice(nullptr) {}

        Device(PhysicalDevice * physicalDevice, const std::set<std::string>& enabledExtensions);

        ~Device();

        Device(const Device&) = delete;

        Device(Device&&) = default;

        Device& operator=(const Device&) = delete;

        Device& operator=(Device&&) = default;

        void waitIdle();

        inline const std::set<std::string>& getEnabledExtensions() const noexcept {
            return _enabledExtensions;
        }

        inline PhysicalDevice * getPhysicalDevice() const noexcept {
            return _physicalDevice;
        }

        inline QueueFamily * getQueueFamily(std::ptrdiff_t index) const noexcept {
            return _queueFamilies.get() + index;
        }

        inline std::uint32_t getQueueFamilyCount() const noexcept {
            return _queueFamilyCount;
        }

        inline VkDevice getHandle() const {
            return _handle;
        }

        std::vector<QueueFamily *> getQueueFamilies() const;

        ShaderModule * getShaderModule(const ShaderModule::CreateInfo& createInfo);
    };
}