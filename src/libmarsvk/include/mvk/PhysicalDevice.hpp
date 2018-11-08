#pragma once

#include "volk.h"

#include "mvk/Device.hpp"
#include "mvk/Format.hpp"
#include "mvk/IndexType.hpp"
#include "mvk/PhysicalDeviceType.hpp"
#include "mvk/Util.hpp"
#include "mvk/Vendor.hpp"

#include <memory>
#include <string>

namespace mvk {
    class PhysicalDevice {
        VkPhysicalDevice _handle;
        VkPhysicalDeviceProperties _properties;
        VkPhysicalDeviceFeatures _features;
        VkPhysicalDeviceMemoryProperties _memoryProperties;
        Format _preferredDepthStencilFormat;
        Vendor _vendor;
        bool _hasUniformMemory;

        PhysicalDevice(const PhysicalDevice&) = delete;

        PhysicalDevice& operator= (const PhysicalDevice&) = delete;

    public:
        PhysicalDevice() noexcept: 
            _handle(VK_NULL_HANDLE) {}

        PhysicalDevice(VkPhysicalDevice handle);

        PhysicalDevice(PhysicalDevice&& from) = default;

        PhysicalDevice& operator= (PhysicalDevice&& from) = default;

        PhysicalDeviceType getPhysicalDeviceType() const noexcept;

        inline std::unique_ptr<Device> createDevice(const std::set<std::string>& enabledExtensions) {
            return std::make_unique<Device> (this, enabledExtensions);
        }

        inline std::unique_ptr<Device> createDevice() {
            return createDevice(std::set<std::string>());
        }

        inline VkPhysicalDevice getHandle() const noexcept {
            return _handle;
        }

        inline const VkPhysicalDeviceProperties& getProperties() const noexcept {
            return _properties;
        }

        inline const VkPhysicalDeviceFeatures& getFeatures() const noexcept {
            return _features;
        }

        inline const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const noexcept {
            return _memoryProperties;
        }

        inline Format getPreferredDepthStencilFormat() const noexcept {
            return _preferredDepthStencilFormat;
        }

        inline Vendor getVendor() const noexcept {
            return _vendor;
        }

        inline bool hasUniformMemory() const noexcept {
            return _hasUniformMemory;
        }

        inline VkDeviceSize alignUpToMinUniformBufferOffset(VkDeviceSize size) const noexcept {
            auto minUboAlignment = _properties.limits.minUniformBufferOffsetAlignment;

            return Util::alignUp(size, minUboAlignment);
        }

        inline VkDeviceSize alignDownToMinUniformBufferOffset(VkDeviceSize size) const noexcept {
            auto minUboAlignment = _properties.limits.minUniformBufferOffsetAlignment;

            return Util::alignDown(size, minUboAlignment);
        }

        inline VkDeviceSize alignUptoMinVertexBufferOffset(VkDeviceSize size) const noexcept {
            return Util::alignUp(size, 4 * sizeof(float));
        }

        inline VkDeviceSize alignDownToVertexBufferOffset(VkDeviceSize size) const noexcept {
            return Util::alignDown(size, 4 * sizeof(float));
        }

        inline VkDeviceSize alignUpToMinTexelBufferOffset(VkDeviceSize size) const noexcept {
            auto minTboAlignment = _properties.limits.minTexelOffset;

            return Util::alignUp(size, minTboAlignment);
        }

        inline VkDeviceSize alignDownToMinTexelBufferOffset(VkDeviceSize size) const noexcept {
            auto minTboAlignment = _properties.limits.minTexelOffset;

            return Util::alignDown(size, minTboAlignment);
        }

        inline VkDeviceSize alignUpToMinIndexBufferOffset(VkDeviceSize size, IndexType indexType) const noexcept {
            switch (indexType) {
                case IndexType::UINT16:
                    return Util::alignUp(size, 2);
                case IndexType::UINT32:
                    return Util::alignUp(size, 4);
                default:
                    __builtin_trap();
            }
        }

        inline VkDeviceSize alignDownToMinIndexBufferOffset(VkDeviceSize size, IndexType indexType) const noexcept {
            switch (indexType) {
                case IndexType::UINT16:
                    return Util::alignDown(size, 2);
                case IndexType::UINT32:
                    return Util::alignDown(size, 4);
                default:
                    __builtin_trap();
            }
        }

        inline VkDeviceSize alignUpToMinStorageBufferOffset(VkDeviceSize size) const noexcept {
            auto minSboAlignment = _properties.limits.minStorageBufferOffsetAlignment;

            return Util::alignUp(size, minSboAlignment);
        }

        inline VkDeviceSize alignDownToMinStorageBufferOffset(VkDeviceSize size) const noexcept {
            auto minSboAlignment = _properties.limits.minStorageBufferOffsetAlignment;

            return Util::alignDown(size, minSboAlignment);
        }

        std::string toString() const noexcept;
    };
}
