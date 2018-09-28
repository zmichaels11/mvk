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

        public:
            PhysicalDevice(): _handle(VK_NULL_HANDLE) {}

            PhysicalDevice(VkPhysicalDevice handle);

            PhysicalDeviceType getPhysicalDeviceType() const;

            std::unique_ptr<Device> createDevice(const std::set<std::string>& enabledExtensions);

            inline VkPhysicalDevice getHandle() const {
                return _handle;
            }

            inline const VkPhysicalDeviceProperties getProperties() const {
                return _properties;
            }

            inline const VkPhysicalDeviceFeatures getFeatures() const {
                return _features;
            }

            inline const VkPhysicalDeviceMemoryProperties getMemoryProperties() const {
                return _memoryProperties;
            }

            inline Format getPreferredDepthStencilFormat() const {
                return _preferredDepthStencilFormat;
            }

            inline Vendor getVendor() const {
                return _vendor;
            }

            inline bool hasUniformMemory() const {
                return _hasUniformMemory;
            }

            inline VkDeviceSize alignUpToMinUniformBufferOffset(VkDeviceSize size) const {
                auto minUboAlignment = _properties.limits.minUniformBufferOffsetAlignment;

                return Util::alignUp(size, minUboAlignment);
            }

            inline VkDeviceSize alignDownToMinUniformBufferOffset(VkDeviceSize size) const {
                auto minUboAlignment = _properties.limits.minUniformBufferOffsetAlignment;

                return Util::alignDown(size, minUboAlignment);
            }

            inline VkDeviceSize alignUptoMinVertexBufferOffset(VkDeviceSize size) const {
                return Util::alignUp(size, 4 * sizeof(float));
            }

            inline VkDeviceSize alignDownToVertexBufferOffset(VkDeviceSize size) const {
                return Util::alignDown(size, 4 * sizeof(float));
            }

            inline VkDeviceSize alignUpToMinTexelBufferOffset(VkDeviceSize size) const {
                auto minTboAlignment = _properties.limits.minTexelOffset;

                return Util::alignUp(size, minTboAlignment);
            }

            inline VkDeviceSize alignDownToMinTexelBufferOffset(VkDeviceSize size) const {
                auto minTboAlignment = _properties.limits.minTexelOffset;

                return Util::alignDown(size, minTboAlignment);
            }

            inline VkDeviceSize alignUpToMinIndexBufferOffset(VkDeviceSize size, IndexType indexType) const {
                switch (indexType) {
                    case IndexType::UINT16:
                        return Util::alignUp(size, 2);
                    case IndexType::UINT32:
                        return Util::alignUp(size, 4);
                    default:
                        __builtin_trap();
                }
            }

            inline VkDeviceSize alignDownToMinIndexBufferOffset(VkDeviceSize size, IndexType indexType) const {
                switch (indexType) {
                    case IndexType::UINT16:
                        return Util::alignDown(size, 2);
                    case IndexType::UINT32:
                        return Util::alignDown(size, 4);
                    default:
                        __builtin_trap();
                }
            }

            inline VkDeviceSize alignUpToMinStorageBufferOffset(VkDeviceSize size) const {
                auto minSboAlignment = _properties.limits.minStorageBufferOffsetAlignment;

                return Util::alignUp(size, minSboAlignment);
            }

            inline VkDeviceSize alignDownToMinStorageBufferOffset(VkDeviceSize size) const {
                auto minSboAlignment = _properties.limits.minStorageBufferOffsetAlignment;

                return Util::alignDown(size, minSboAlignment);
            }

            std::string toString() const;
    };
}
