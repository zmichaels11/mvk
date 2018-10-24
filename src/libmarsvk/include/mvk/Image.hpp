#pragma once

#include "volk.h"
#include "vk_mem_alloc.h"

#include <functional>
#include <memory>
#include <utility>
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

    class Image;

    using UPtrImage = std::unique_ptr<Image>;

    class Image {
    public:
        //! Parameter structure specifying how to construct a new Image object.
        struct CreateInfo {
            unsigned int flags;                         /*!< Bitmask of additional parameters */
            ImageType imageType;                        /*!< The basic dimensionality of the Image. Layers do not count toward number of dimensions. */
            Format format;                              /*!< The type and format of data elements. */
            Extent3D extent;                            /*!< The number of data elements for each dimension at the base level. */
            int mipLevels;                              /*!< The number of levels of detail for minified sampling. */
            int arrayLayers;                            /*!< The number of layers in the image. */
            int samples;                                /*!< The number of sub-data element samples. */
            ImageTiling tiling;                         /*!< The tiling arrangement of data elements in memory. */
            ImageUsageFlag usage;                       /*!< Bitmask of usage limitations. */
            SharingMode sharingMode;                    /*!< QueueFamily Sharing mode limitation */
            std::vector<QueueFamily *> queueFamilies;   /*!< Set of QueueFamilies that can access the Image. */
            ImageLayout initialLayout;                  /*!< Initial Layout of the Image. This should be UNDEFINED unless the image was constructed as LINEAR tiling. */
            bool exported;                              /*!< Specifies if the backing memory should be exported */
        };

        //! Constructs a Image-typed unique_ptr pointing to nothing.
        /*!
            \return unique_ptr<Image> pointing to nullptr.
         */
        static inline UPtrImage unique_null() noexcept {
            return std::unique_ptr<Image>();
        }

    private:
        Device * _device;
        VkImage _handle;
        CreateInfo _info;
        bool _external;

        union {
            VkDeviceMemory shared;
            VmaAllocation local;
        } _memory;

        Image(const Image&) = delete;
        Image& operator= (const Image&) = delete;

    public:
        //! User-specified pointer
        std::shared_ptr<void> userData;

        //! Constructs an Image wrapping nothing.
        Image() noexcept:
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}

        //! Constructs a new Image object by wrapping a Vulkan Image handle.
        /*!
            \param device is Vulkan Device used to construct the Image.
            \param createInfo is the parameter struct defining how the Image was constructed.
            \param handle is the handle to wrap.
        */
        Image(Device * device, const CreateInfo& createInfo, VkImage handle) noexcept:
            _device(device),
            _handle(handle),
            _info(createInfo),
            _external(true) {}
        
        //! Constructs a new Image object.
        /*!
            \param device is the Vulkan Device used to construct the Image.
            \param createInfo is the parameter struct defining how to construct the Image.
            \param memoryUsage is the preferred memory rules for allocating backing memory.
        */
        Image(Device * device, const CreateInfo& createInfo, MemoryUsage memoryUsage);     

        //! Deletes the Image and frees all underlying resources.
        ~Image() noexcept;

        //! Move constructs the Image.
        /*!
            \param from the other Image.
        */
        Image(Image&& from) noexcept;

        //! Move-assigns the Image.
        /*!
            \param from the other Image.
        */
        Image& operator= (Image&& from) noexcept;

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkImage() const noexcept {
            return _handle;
        }        

        //! Retrieves the parent Device object.
        /*!
            \return the parent Device object.
        */
        inline Device * getDevice() const noexcept {
            return _device;
        }

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Vulkan Image handle.
        */
        inline VkImage getHandle() const noexcept {
            return _handle;
        }

        //! Retrieves the parameter structure used to construct this Image.
        /*!
            \return the reference to an immutable copy of the initial construction parameter struct.
        */
        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        //! Retrieves an ImageSubresourceRange for all layers and levels of this Image.
        /*!
            \return the full ImageSubresourceRange.
        */
        inline const ImageSubresourceRange getFullRange() const noexcept {
            auto range = ImageSubresourceRange {};

            range.aspectMask = Util::aspect(_info.format);
            range.baseMipLevel = 0;
            range.levelCount = _info.mipLevels;
            range.baseArrayLayer = 0;
            range.layerCount = _info.arrayLayers;

            return range;
        }

        //! Retrieves all layers of a specific level of detail.
        /*!
            \param mipLevel the mipmap level
            \return the ImageSubresourceLayers covering all array layers for a specific level of detail.
        */
        inline const ImageSubresourceLayers getMipAllLayers(int mipLevel) const noexcept {
            auto layers = ImageSubresourceLayers {};

            layers.aspectMask = Util::aspect(_info.format);
            layers.baseArrayLayer = 0;
            layers.mipLevel = mipLevel;
            layers.layerCount = _info.arrayLayers;

            return layers;
        }

        //! Retrieves a range of ImageSubresourceLayers.
        /*!
            \param mipLevel is the level of detail to query
            \param baseArrayLayer is the first array layer; zero-base index
            \param layerCount is the number of layers
            \return ImageSubresourceLayers is the specified layer range.
        */
        inline const ImageSubresourceLayers getSubresourceLayers(int mipLevel, int baseArrayLayer = 0, int layerCount = 1) const noexcept {
            auto layers = ImageSubresourceLayers {};

            layers.aspectMask = Util::aspect(_info.format);
            layers.baseArrayLayer = baseArrayLayer;
            layers.mipLevel = mipLevel;
            layers.layerCount = layerCount;

            return layers;
        }

        //! Retrieves a range of ImageSubresourceRange.
        /*!
            \param baseMipLevel is the highest level of detail
            \param levelCount is the number of mipmap levels to span
            \param baseArrayLayer is the lowest array layer
            \param layercount is the total number of layers to span
            \return ImageSubresourceRange the entire specified range.
        */
        inline const ImageSubresourceRange getSubresourceRange(int baseMipLevel, int levelCount = 1, int baseArrayLayer = 0, int layerCount = 1) const noexcept {
            auto range = ImageSubresourceRange {};

            range.aspectMask = Util::aspect(_info.format);
            range.baseMipLevel = baseMipLevel;
            range.levelCount = levelCount;
            range.baseArrayLayer = baseArrayLayer;
            range.layerCount = layerCount;

            return range;
        }

        //! Checks if the Image is an array Image
        /*!
            \return true if the Image has more than 1 layer.
        */
        inline bool isArrayed() const noexcept {
            return _info.arrayLayers > 1;
        }

        //! Checks if the Image has multiple levels of detail.
        /*!
            \return true if the Image has more than the base level mipmap.
        */
        inline bool hasMipmap() const noexcept {
            return _info.mipLevels > 1;
        }
        
        int getFd() const;
    };
}
