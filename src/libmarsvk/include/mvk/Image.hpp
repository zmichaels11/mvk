#pragma once

#include "volk.h"
#include "vk_mem_alloc.h"

#include <functional>
#include <vector>

#include "mvk/Extent3D.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/ImageSubresourceRange.hpp"
#include "mvk/ImageTiling.hpp"
#include "mvk/ImageType.hpp"
#include "mvk/ImageUsageFlag.hpp"
#include "mvk/MemoryUsage.hpp"
#include "mvk/SharingMode.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    class Device;
    class QueueFamily;

    class Image {
    public:
        struct CreateInfo {
            unsigned int flags;
            ImageType imageType;
            Format format;
            Extent3D extent;
            int mipLevels;
            int arrayLayers;
            int samples;
            ImageTiling tiling;
            ImageUsageFlag usage;
            SharingMode sharingMode;
            std::vector<QueueFamily *> queueFamilies;
            ImageLayout initialLayout;
        };

    private:
        Device * _device;
        VkImage _handle;
        CreateInfo _info;
        VmaAllocation _memory;

    public:
        void * userData;

        Image() :
            _device(nullptr),
            _handle(VK_NULL_HANDLE),
            _memory(VK_NULL_HANDLE) {}

        Image(Device * device, const CreateInfo& createInfo, VkImage handle) :
            _device(device),
            _handle(handle),
            _info(createInfo),
            _memory(VK_NULL_HANDLE) {}
        
        Image(Device * device, const CreateInfo& createInfo, MemoryUsage memoryUsage);

        Image(const Image&) = delete;

        Image(Image&&) = default;

        Image& operator= (const Image&) = delete;

        Image& operator= (Image&&) = default;

        ~Image();

        inline Device * getDevice() const noexcept {
            return _device;
        }

        inline VkImage getHandle() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline const ImageSubresourceRange getFullRange() const noexcept {
            auto range = ImageSubresourceRange {};

            range.aspectMask = Util::aspect(_info.format);
            range.baseMipLevel = 0;
            range.levelCount = _info.mipLevels;
            range.baseArrayLayer = 0;
            range.layerCount = _info.arrayLayers;

            return range;
        }

        inline const ImageSubresourceLayers getMipAllLayers(int mipLevel) const noexcept {
            auto layers = ImageSubresourceLayers {};

            layers.aspectMask = Util::aspect(_info.format);
            layers.baseArrayLayer = 0;
            layers.mipLevel = mipLevel;
            layers.layerCount = _info.arrayLayers;

            return layers;
        }

        inline const ImageSubresourceLayers getSubresourceLayers(int mipLevel, int baseArrayLayer = 0, int layerCount = 1) const noexcept {
            auto layers = ImageSubresourceLayers {};

            layers.aspectMask = Util::aspect(_info.format);
            layers.baseArrayLayer = baseArrayLayer;
            layers.mipLevel = mipLevel;
            layers.layerCount = layerCount;

            return layers;
        }

        inline const ImageSubresourceRange getSubresourceRange(int baseMipLevel, int levelCount = 1, int baseArrayLayer = 0, int layerCount = 1) const noexcept {
            auto range = ImageSubresourceRange {};

            range.aspectMask = Util::aspect(_info.format);
            range.baseMipLevel = baseMipLevel;
            range.levelCount = levelCount;
            range.baseArrayLayer = baseArrayLayer;
            range.layerCount = layerCount;

            return range;
        }

        inline bool isArrayed() const noexcept {
            return _info.arrayLayers > 1;
        }

        inline bool hasMipmap() const noexcept {
            return _info.mipLevels > 1;
        }

        void * map();

        void unmap();

        inline void mapping(std::function<void(void*)> fn) {
            fn(map());
            unmap();
        }
    };
}
