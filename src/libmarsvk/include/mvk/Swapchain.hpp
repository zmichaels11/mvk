#pragma once

#include "volk.h"

#include "mvk/PresentMode.hpp"
#include "mvk/SurfaceFormat.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace mvk {
    class Device;
    class Image;
    class QueueFamily;
    class Semaphore;
    class Surface;

    class Swapchain;
    using UPtrSwapchain = std::unique_ptr<Swapchain>;

    class Swapchain {
        struct Support {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            std::vector<VkPresentModeKHR> presentModes;

            Support() noexcept {}

            Support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) noexcept;
        };

    public:
        struct CreateInfo {
            Surface * surface;
            QueueFamily * queueFamily;
            SurfaceFormat surfaceFormat;
            PresentMode presentMode;
        };

        struct Backbuffer {
            Image * image;
            Semaphore * acquireSemaphore;
            int index;

            Backbuffer(Image * image, Semaphore * acquireSemaphore, int index) noexcept:
                image(image),
                acquireSemaphore(acquireSemaphore),
                index(index) {}
        };    

        static inline UPtrSwapchain unique_null() noexcept {
            return std::unique_ptr<Swapchain> ();
        }

    private:
        CreateInfo _info;
        Device * _device;
        VkSwapchainKHR _handle;
        VkPresentModeKHR _presentMode;
        Support _support;
        int _width;
        int _height;
        std::vector<std::unique_ptr<Image>> _images;

        Swapchain(const Swapchain&) = delete;
        Swapchain& operator= (const Swapchain&) = delete;

    public:
        void * userData;

        Swapchain() noexcept:
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}

        Swapchain(Device * device, const CreateInfo& createInfo) noexcept:
            _info(createInfo),
            _device(device),
            _handle(VK_NULL_HANDLE),
            _presentMode(VK_PRESENT_MODE_FIFO_KHR),
            _support(),
            _width(0),
            _height(0),
            _images() {}

        Swapchain(Swapchain&& from) noexcept:
            _info(std::move(from._info)),
            _device(std::move(from._device)),
            _handle(std::exchange(from._handle, nullptr)),
            _presentMode(std::move(from._presentMode)),
            _support(std::move(from._support)),
            _width(std::move(from._width)),
            _height(std::move(from._height)),
            _images(std::move(from._images)) {}

        ~Swapchain() noexcept;

        Swapchain& operator= (Swapchain&& from) noexcept;

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
