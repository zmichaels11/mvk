#include "mvk/Instance.hpp"

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <set>

#include "volk.h"
#include <GLFW/glfw3.h>

#include "mvk/PhysicalDevice.hpp"

namespace mvk {
    namespace {
        std::set<std::string> _enabledLayers;
        std::set<std::string> _enabledExtensions;
    }

    void Instance::enableExtension(const std::string& extName) noexcept {
        _enabledExtensions.insert(extName);
    }

    void Instance::enableLayer(const std::string& layerName) noexcept {
        _enabledLayers.insert(layerName);
    }

    void Instance::enableRequiredGLFWExtensions() noexcept {
        uint32_t requiredGLFWExtensions = 0;
        auto extensions = glfwGetRequiredInstanceExtensions(&requiredGLFWExtensions);

        std::for_each(extensions, extensions + requiredGLFWExtensions, [](auto extName) {
            _enabledExtensions.insert(extName);
        });
    }

    Instance::Instance() {
        if (VK_SUCCESS != volkInitialize()) {
            throw std::runtime_error("Volk could not be initialized!");
        }

        auto applicationInfo = VkApplicationInfo {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
        applicationInfo.pApplicationName = "MVKApp";
        applicationInfo.pEngineName = "mvk";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        
#if defined(DEBUG) || defined(NDEBUG) || defined(_DEBUG)
        _enabledLayers.insert("VK_LAYER_LUNARG_standard_validation");
#endif

        auto pEnabledExtensionNames = std::vector<const char *> ();
        pEnabledExtensionNames.reserve(_enabledExtensions.size());

        for (auto& name : _enabledExtensions) {
            pEnabledExtensionNames.push_back(name.c_str());
        }
        
        auto pEnabledLayers = std::vector<const char *> ();
        pEnabledLayers.reserve(_enabledLayers.size());

        for (auto& layerName : _enabledLayers) {
            pEnabledLayers.push_back(layerName.c_str());
        }

        auto instanceCI = VkInstanceCreateInfo{};
        instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCI.pApplicationInfo = &applicationInfo;
        instanceCI.ppEnabledExtensionNames = pEnabledExtensionNames.data();
        instanceCI.enabledExtensionCount = pEnabledExtensionNames.size();
        instanceCI.ppEnabledLayerNames = pEnabledLayers.data();
        instanceCI.enabledLayerCount = pEnabledLayers.size();

        Util::vkAssert(vkCreateInstance(&instanceCI, nullptr, &_handle));
        
        volkLoadInstance(_handle);

        std::uint32_t physicalDeviceCount = 0;
        Util::vkAssert(vkEnumeratePhysicalDevices(_handle, &physicalDeviceCount, nullptr));

        auto pPhysicalDevices = std::make_unique<VkPhysicalDevice[]> (physicalDeviceCount);

        Util::vkAssert(vkEnumeratePhysicalDevices(_handle, &physicalDeviceCount, pPhysicalDevices.get()));

        _physicalDevices.reserve(physicalDeviceCount);

        for (std::uint32_t i = 0; i < physicalDeviceCount; i++) {
            _physicalDevices.push_back(PhysicalDevice(pPhysicalDevices[i]));
        }
    }

    Instance::~Instance() noexcept {
        free();
    }

    Instance& Instance::operator= (Instance&& from) noexcept {
        std::swap(this->_handle, from._handle);
        std::swap(this->_physicalDevices, from._physicalDevices);

        return *this;
    }

    void Instance::free() noexcept {
        if (_handle) {
            vkDestroyInstance(_handle, nullptr);
            _handle = VK_NULL_HANDLE;
        }
    }

    Instance& Instance::getCurrent() {
        static Instance THE_INSTANCE;

        return THE_INSTANCE;
    }
}
