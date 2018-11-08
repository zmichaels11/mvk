#include "mvk/Surface.hpp"

#include "mvk/Instance.hpp"

namespace mvk {
    Surface::Surface(Instance& instance, GLFWwindow * window) noexcept {
        glfwCreateWindowSurface(instance, window, nullptr, &_handle);
        _instance = &instance;
    }

    Surface::~Surface() noexcept {
        vkDestroySurfaceKHR(_instance->getHandle(), _handle, nullptr);
    }

    Surface& Surface::operator= (Surface&& from) noexcept {
        std::swap(this->_handle, from._handle);
        std::swap(this->_instance, from._instance);

        return *this;
    }
}
