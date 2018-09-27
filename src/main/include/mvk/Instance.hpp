#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"

#include <memory>
#include <string>

#include "mvk/PhysicalDevice.hpp"

namespace mvk {
    class Instance {
        VkInstance _handle;
        std::unique_ptr<PhysicalDevice[]> _physicalDevices;
        std::uint32_t _physicalDeviceCount;

        Instance();

        Instance(const Instance&) = delete;

        Instance& operator= (const Instance&) = delete;
        
        public:
            static void enableLayer(const std::string& layer);

            static void enableRequiredGLFWExtensions();

            static void enableExtension(const std::string& extension);

            static Instance& getCurrent();

            void free();

            inline const VkInstance getHandle() const {
                return _handle;
            }

            inline std::uint32_t getPhysicalDeviceCount() const {
                return _physicalDeviceCount;
            }

            inline const PhysicalDevice * getPhysicalDevice(std::ptrdiff_t index) const {
                return _physicalDevices.get() + index;
            }
    };
}