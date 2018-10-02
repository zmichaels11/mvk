#pragma once

#include "volk.h"

#include <vector>

namespace mvk {
    class Device;
    class Image;
    class QueueFamily;
    class Semaphore;

    class Swapchain {
        struct Support {
            VkSurfaceCapabilitiesKHR capabilities;
            VkSurfaceFormatKHR surfaceFormats;
            std::vector<VkPresentModeKHR> presentModes;
        };

    public:
        struct CreateInfo {
            VkSurfaceKHR surface;
            QueueFamily * queueFamily;
            int width;
            int height;
        };

        struct Backbuffer {
            Image * image;
            Semaphore * acquireSemaphore;
            int index;
        };

    private:
        CreateInfo _info;
        Device * _device;
        VkSwapchainKHR _handle;
        VkPresentModeKHR _presentMode;
        Support _support;
        int _width;
        int _height;
        std::vector<Image *> _images;

    public:
        void * userData;

        Swapchain() :
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}

        Swapchain(Device * device, const CreateInfo& createInfo);

        Swapchain(const Swapchain&) = delete;

        Swapchain(Swapchain&&) = default;

        ~Swapchain();

        Swapchain& operator= (const Swapchain&) = delete;

        Swapchain& operator= (Swapchain&&) = default;

        inline int getWidth() const noexcept {
            return _width;
        }

        inline int getHeight() const noexcept {
            return _height;
        }

        inline const std::vector<Image *>& getImages() const noexcept {
            return _images;
        }

        inline VkSwapchainKHR getHandle() const noexcept {
            return _handle;
        }

        inline Device * getDevice() const noexcept {
            return _device;
        }

        Backbuffer acquireNextImage();

        void resize(int width, int height);
    };
}
