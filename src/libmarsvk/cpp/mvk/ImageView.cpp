#include "mvk/ImageView.hpp"

#include <cstdint>

#include <stdexcept>

#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    ImageView::ImageView(const Image * image, const CreateInfo& createInfo) {
        _image = image;
        _info = createInfo;

        auto pDevice = image->getDevice();

        auto imageViewCI = VkImageViewCreateInfo {};
        imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCI.image = image->getHandle();
        imageViewCI.viewType = static_cast<VkImageViewType> (createInfo.viewType);
        imageViewCI.format = static_cast<VkFormat> (createInfo.format);
        imageViewCI.subresourceRange.aspectMask = static_cast<VkImageAspectFlags> (createInfo.subresourceRange.aspectMask);
        imageViewCI.subresourceRange.baseMipLevel = static_cast<std::uint32_t> (createInfo.subresourceRange.baseMipLevel);
        imageViewCI.subresourceRange.levelCount = static_cast<std::uint32_t> (createInfo.subresourceRange.levelCount);
        imageViewCI.subresourceRange.baseArrayLayer = static_cast<std::uint32_t> (createInfo.subresourceRange.baseArrayLayer);
        imageViewCI.subresourceRange.layerCount = static_cast<std::uint32_t> (createInfo.subresourceRange.layerCount);

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateImageView(pDevice->getHandle(), &imageViewCI, nullptr, &_handle));
    }

    ImageView::ImageView(const Image * image) {
        auto createInfo = CreateInfo {};
        createInfo.format = image->getInfo().format;
        createInfo.subresourceRange = image->getFullRange();
        
        if (image->isArrayed()) {
            switch (image->getInfo().imageType) {
                case ImageType::IMAGE_1D:
                    createInfo.viewType = ImageViewType::VIEW_1D_ARRAY;
                    break;
                case ImageType::IMAGE_2D:
                    createInfo.viewType = ImageViewType::VIEW_2D_ARRAY;
                    break;
                default:
                    throw std::runtime_error("Only 1D and 2D images can be arrayed!");
            }
        } else {
            switch (image->getInfo().imageType) {
                case ImageType::IMAGE_1D:
                    createInfo.viewType = ImageViewType::VIEW_1D;
                    break;
                case ImageType::IMAGE_2D:
                    createInfo.viewType = ImageViewType::VIEW_2D;
                    break;
                case ImageType::IMAGE_3D:
                    createInfo.viewType = ImageViewType::VIEW_3D;
                    break;
                default:
                    throw std::runtime_error("ImageView::CreateInfo can only be derived from 1D, 2D, or 3D Images!");
            }
        }

        _image = image;
        _info = createInfo;

        auto pDevice = image->getDevice();

        auto imageViewCI = VkImageViewCreateInfo {};
        imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCI.image = image->getHandle();
        imageViewCI.viewType = static_cast<VkImageViewType> (createInfo.viewType);
        imageViewCI.format = static_cast<VkFormat> (createInfo.format);
        imageViewCI.subresourceRange.aspectMask = static_cast<VkImageAspectFlags> (createInfo.subresourceRange.aspectMask);
        imageViewCI.subresourceRange.baseMipLevel = static_cast<std::uint32_t> (createInfo.subresourceRange.baseMipLevel);
        imageViewCI.subresourceRange.levelCount = static_cast<std::uint32_t> (createInfo.subresourceRange.levelCount);
        imageViewCI.subresourceRange.baseArrayLayer = static_cast<std::uint32_t> (createInfo.subresourceRange.baseArrayLayer);
        imageViewCI.subresourceRange.layerCount = static_cast<std::uint32_t> (createInfo.subresourceRange.layerCount);

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateImageView(pDevice->getHandle(), &imageViewCI, nullptr, &_handle));
    }

    ImageView::~ImageView() noexcept {
        vkDestroyImageView(getDevice()->getHandle(), _handle, nullptr);
    }

    ImageView& ImageView::operator= (ImageView&& from) noexcept {
        std::swap(this->_handle, from._handle);
        std::swap(this->_image, from._image);
        std::swap(this->_info, from._info);

        return *this;
    }

    Device * ImageView::getDevice() const noexcept {
        return _image->getDevice();
    }
}