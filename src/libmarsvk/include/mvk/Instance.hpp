#pragma once

#include <cstddef>
#include <cstdint>

#include "volk.h"

#include <memory>
#include <string>
#include <vector>

#include "mvk/InstanceLayer.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    class Instance {
        VkInstance _handle;
        std::unique_ptr<PhysicalDevice[]> _physicalDevices;
        std::uint32_t _physicalDeviceCount;

        Instance();

        ~Instance();

        Instance(const Instance&) = delete;

        Instance(Instance&&) = default;

        Instance& operator= (const Instance&) = delete;

        Instance& operator= (Instance&&) = default;
        
        public:
            static void enableLayer(const std::string& layer);

            inline static void enableLayer(const InstanceLayer& layer) {
                enableLayer(Util::name(layer));
            }

            static void enableRequiredGLFWExtensions();

            static void enableExtension(const std::string& extension);

            static Instance& getCurrent();

            void free();

            inline VkInstance getHandle() const {
                return _handle;
            }

            inline std::uint32_t getPhysicalDeviceCount() const {
                return _physicalDeviceCount;
            }

            inline PhysicalDevice * getPhysicalDevice(std::ptrdiff_t index) const {
                return _physicalDevices.get() + index;
            }

            std::vector<PhysicalDevice *> getPhysicalDevices() const;
    };
}