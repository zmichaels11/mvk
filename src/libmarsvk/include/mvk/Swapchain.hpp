#pragma once

#include "volk.h"

#include "mvk/SurfaceFormat.hpp"

#include <memory>
#include <vector>

namespace mvk {
    class Device;
    class Image;
    class QueueFamily;
    class Semaphore;

    class Swapchain {
        struct Support {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            std::vector<VkPresentModeKHR> presentModes;

            Support() {}

            Support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
        };

    public:
        struct CreateInfo {
            VkSurfaceKHR surface;
            QueueFamily * queueFamily;
            SurfaceFormat surfaceFormat;
        };

        struct Backbuffer {
            Image * image;
            Semaphore * acquireSemaphore;
            int index;

            Backbuffer(Image * image, Semaphore * acquireSemaphore, int index) :
                image(image),
                acquireSemaphore(acquireSemaphore),
                index(index) {}
        };

    private:
        CreateInfo _info;
        Device * _device;
        VkSwapchainKHR _handle;
        VkPresentModeKHR _presentMode;
        Support _support;
        int _width;
        int _height;
        std::vector<std::unique_ptr<Image>> _images;

    public:
        void * userData;

        Swapchain() :
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}

        Swapchain(Device * device, const CreateInfo& createInfo) :
            _info(createInfo),
            _device(device),
            _handle(VK_NULL_HANDLE),
            _presentMode(VK_PRESENT_MODE_FIFO_KHR),
            _support(),
            _width(0),
            _height(0),
            _images() {}

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

        inline const std::vector<Image *> getImages() const noexcept {
            auto images = std::vector<Image *> ();

            images.reserve(_images.size());
            for (const auto& image : _images) {
                images.push_back(image.get());
            }

            return images;
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
