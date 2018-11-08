#pragma once

#include "volk.h"
#include <GLFW/glfw3.h>

#include <utility>

namespace mvk {
    class Instance;
    
    class Surface {
        VkSurfaceKHR _handle;
        Instance * _instance;

        Surface(const Surface&) = delete;
        Surface& operator= (const Surface&&) = delete;

    public:
        Surface() noexcept:
            _handle(VK_NULL_HANDLE),
            _instance(nullptr) {}
            
        Surface(Instance& instance, GLFWwindow * window) noexcept;

        Surface(Surface&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _instance(std::move(from._instance)) {}

        ~Surface() noexcept;

        Surface& operator= (Surface&& from) noexcept;

        inline VkSurfaceKHR getHandle() const noexcept {
            return _handle;
        }

        inline operator VkSurfaceKHR() const noexcept {
            return _handle;
        }
    };
}
