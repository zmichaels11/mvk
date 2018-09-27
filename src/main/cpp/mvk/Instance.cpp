#include "mvk/Instance.hpp"

#include <cstdint>

#include <algorithm>
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
        VkApplicationInfo applicationInfo {};

        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
        applicationInfo.pApplicationName = "MVKApp";
        applicationInfo.pEngineName = "mvk";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        
        std::vector<const char *> pEnabledExtensionNames;

        std::for_each(_enabledExtensions.begin(), _enabledExtensions.end(), [&](const auto& name) { pEnabledExtensionNames.push_back(name.c_str()); });
        
        std::vector<const char *> pEnabledLayers;

        std::for_each(_enabledLayers.begin(), _enabledLayers.end(), [&](const auto & name) { pEnabledLayers.push_back(name.c_str(); });
        

        VkInstanceCreateInfo instanceCI {};

        instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCI.pApplicationInfo = &applicationInfo;
        instanceCI.ppEnabledExtensionNames = pEnabledExtensionNames.data();
        instanceCI.ppEnabledLayerNames = pEnabledLayers.data();

        vkCreateInstance(&instanceCI, nullptr, &_handle);

        vkEnumeratePhysicalDevices(_handle, &_physicalDeviceCount, nullptr);

        auto pPhysicalDevices = std::make_unique<VkPhysicalDevice[]> (_physicalDeviceCount);

        vkEnumeratePhysicalDevices(_handle, &_physicalDeviceCount, pPhysicalDevices.get());

        std::for_each(pPhysicalDevices.get(), pPhysicalDevices.get() + _physicalDeviceCount, [&](auto physicalDevice) {
            _physicalDevices.push_back(PhysicalDevice(physicalDevice));
        });
    }

    void Instance::free() {
        vkDestroyInstance(_handle, nullptr);
    }

    Instance& Instance::getCurrent() {
        static Instance THE_INSTANCE;

        return THE_INSTANCE;
    }
}