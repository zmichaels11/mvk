#include "mvk/Swapchain.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "mvk/ColorSpace.hpp"
#include "mvk/Device.hpp"
#include "mvk/Format.hpp"
#include "mvk/Image.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        VkSurfaceFormatKHR chooseFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, Format format, ColorSpace colorSpace) {
            if (1 == availableFormats.size() && VK_FORMAT_UNDEFINED == availableFormats[0].format) {
                auto defFormat = VkSurfaceFormatKHR {};
                defFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
                defFormat.colorSpace= VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

                return defFormat;
            }

            for (const auto& surfaceFormat : availableFormats) {
                if (static_cast<VkFormat> (format) == surfaceFormat.format && static_cast<VkColorSpaceKHR> (colorSpace) == surfaceFormat.colorSpace) {
                    return surfaceFormat;
                }
            }

            return availableFormats[0];
        }

        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, VkPresentModeKHR presentMode) {
            for (const auto& presentMode : availablePresentModes) {
                if (presentMode == presentMode) {
                    return presentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }
    }

    Swapchain::Support::Support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        uint32_t formatCount = 0;

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        surfaceFormats.resize(formatCount);

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

        uint32_t presentModeCount = 0;

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        presentModes.resize(presentModeCount);

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
    }

    Swapchain::~Swapchain() {
        if (VK_NULL_HANDLE != _handle) {
            vkDestroySwapchainKHR(getDevice()->getHandle(), _handle, nullptr);
        }
    }

    Swapchain::Backbuffer Swapchain::acquireNextImage() {
        auto pDevice = getDevice();
        auto imageAcquireSemaphore = pDevice->acquireSemaphore();
        auto fence = pDevice->acquireFence();

        uint32_t imageIndex = 0;
        auto result = vkAcquireNextImageKHR(pDevice->getHandle(), _handle, ~0UL, imageAcquireSemaphore->getHandle(), fence->getHandle(), &imageIndex);

        while (!fence->isSignaled()) {
            using namespace std::chrono_literals;

            std::this_thread::sleep_for(1ms);
        }

        fence->reset();
        fence->release();

        if (VK_ERROR_OUT_OF_DATE_KHR == result) {
            pDevice->waitIdle();
            resize(_width, _height);

            return acquireNextImage();
        } else {
            Util::vkAssert(result);
        }

        return Backbuffer(_images[imageIndex].get(), imageAcquireSemaphore, imageIndex);
    }

    void Swapchain::resize(int width, int height) {
        auto pDevice = getDevice();
        auto pPhysicalDevice = pDevice->getPhysicalDevice();

        _support = Support(pPhysicalDevice->getHandle(), _info.surface);

        auto surfaceFormat = chooseFormat(_support.surfaceFormats, _info.surfaceFormat.format, _info.surfaceFormat.colorSpace);
        auto presentMode = choosePresentMode(_support.presentModes, VK_PRESENT_MODE_FIFO_RELAXED_KHR);

        uint32_t imageCount = 0;

        if (_support.capabilities.maxImageCount > 0 && _support.capabilities.minImageCount + 1 > _support.capabilities.maxImageCount) {
            imageCount = _support.capabilities.maxImageCount;
        } else {
            imageCount = _support.capabilities.minImageCount + 1;
        }

        VkSurfaceTransformFlagBitsKHR preTransform;

        if (VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR == (_support.capabilities.currentTransform & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)) {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            preTransform = _support.capabilities.currentTransform;
        }

        auto extent = _support.capabilities.currentExtent;
        auto minExtent = _support.capabilities.minImageExtent;
        auto maxExtent = _support.capabilities.maxImageExtent;

        if (~0u == extent.width || ~0u == extent.height) {
            extent.width = static_cast<uint32_t> (width);
            extent.height = static_cast<uint32_t> (height);
        }

        extent.width = Util::clamp(extent.width, minExtent.width, maxExtent.width);
        extent.height = Util::clamp(extent.height, minExtent.height, maxExtent.height);

        _width = extent.width;
        _height = extent.height;

        auto swapchainCI = VkSwapchainCreateInfoKHR {};
        swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCI.surface = _info.surface;
        swapchainCI.minImageCount = imageCount;
        swapchainCI.imageFormat = surfaceFormat.format;
        swapchainCI.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCI.preTransform = preTransform;
        swapchainCI.imageArrayLayers = 1;
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCI.pQueueFamilyIndices = nullptr;
        swapchainCI.presentMode = presentMode;
        swapchainCI.oldSwapchain = _handle;
        swapchainCI.clipped = VK_TRUE;
        swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCI.imageExtent = extent;
        swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        VkSwapchainKHR newHandle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateSwapchainKHR(pDevice->getHandle(), &swapchainCI, nullptr, &newHandle));

        if (VK_NULL_HANDLE != _handle) {
            vkDestroySwapchainKHR(pDevice->getHandle(), _handle, nullptr);
        }

        _handle = newHandle;

        vkGetSwapchainImagesKHR(pDevice->getHandle(), _handle, &imageCount, nullptr);

        auto imageHandles = std::vector<VkImage> ();
        imageHandles.resize(imageCount);

        vkGetSwapchainImagesKHR(pDevice->getHandle(), _handle, &imageCount, imageHandles.data());

        _images.clear();
        _images.reserve(imageCount);

        for (const auto& handle : imageHandles) {
            auto imageCI = Image::CreateInfo {};
            imageCI.imageType = ImageType::IMAGE_2D;
            imageCI.format = static_cast<Format> (surfaceFormat.format);
            imageCI.extent.width = _width;
            imageCI.extent.height = _height;
            imageCI.extent.depth = 1;
            imageCI.tiling = ImageTiling::OPTIMAL;
            imageCI.usage = ImageUsageFlag::COLOR_ATTACHMENT | ImageUsageFlag::TRANSFER_DST;
            imageCI.sharingMode = SharingMode::EXCLUSIVE;
            imageCI.initialLayout = ImageLayout::PRESENT_SRC_KHR;
            imageCI.mipLevels = 1;
            imageCI.arrayLayers = 1;

            _images.push_back(std::make_unique<Image> (pDevice, imageCI, handle));
        }
    }
}
