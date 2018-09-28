#include "mvk/Instance.hpp"

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "volk.h"

#include "mvk/PhysicalDevice.hpp"

namespace mvk {
    namespace {
        std::vector<std::string> _enabledLayers;
        std::vector<std::string> _enabledExtensions;
    }

    void Instance::enableExtension(const std::string& extName) {
        _enabledExtensions.push_back(extName);
    }

    void Instance::enableLayer(const std::string& layerName) {
        _enabledLayers.push_back(layerName);
    }

    Instance::Instance() {
        if (VK_SUCCESS != volkInitialize()) {
            throw std::runtime_error("Volk could not be initialized!");
        }

        VkApplicationInfo applicationInfo {};

        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
        applicationInfo.pApplicationName = "MVKApp";
        applicationInfo.pEngineName = "mvk";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        
        std::vector<const char *> pEnabledExtensionNames;

        for (auto& name : _enabledExtensions) {
            pEnabledExtensionNames.push_back(name.c_str());
        }
        
        std::vector<const char *> pEnabledLayers;

        for (auto& layerName : _enabledLayers) {
            pEnabledLayers.push_back(layerName.c_str());
        }

        VkInstanceCreateInfo instanceCI {};

        instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCI.pApplicationInfo = &applicationInfo;
        instanceCI.ppEnabledExtensionNames = pEnabledExtensionNames.data();
        instanceCI.enabledExtensionCount = pEnabledExtensionNames.size();
        instanceCI.ppEnabledLayerNames = pEnabledLayers.data();
        instanceCI.enabledLayerCount = pEnabledLayers.size();

        Util::vkAssert(vkCreateInstance(&instanceCI, nullptr, &_handle));
        
        volkLoadInstance(_handle);

        Util::vkAssert(vkEnumeratePhysicalDevices(_handle, &_physicalDeviceCount, nullptr));

        auto pPhysicalDevices = std::make_unique<VkPhysicalDevice[]> (_physicalDeviceCount);

        Util::vkAssert(vkEnumeratePhysicalDevices(_handle, &_physicalDeviceCount, pPhysicalDevices.get()));

        _physicalDevices = std::make_unique<PhysicalDevice[]> (_physicalDeviceCount);

        for (std::uint32_t i = 0; i < _physicalDeviceCount; i++) {
            auto pd = PhysicalDevice(pPhysicalDevices[i]);
            
            std::swap(_physicalDevices[i], pd);
        }
    }

    Instance::~Instance() {
        vkDestroyInstance(_handle, nullptr);
    }

    void Instance::free() {
        vkDestroyInstance(_handle, nullptr);
    }

    Instance& Instance::getCurrent() {
        static Instance THE_INSTANCE;

        return THE_INSTANCE;
    }

    std::vector<PhysicalDevice * > Instance::getPhysicalDevices() const {
        auto out = std::vector<PhysicalDevice * >();

        out.reserve(_physicalDeviceCount);

        for (std::uint32_t i = 0; i < _physicalDeviceCount; i++) {
            out.push_back(_physicalDevices.get() + i);
        }

        return out;
    }
}