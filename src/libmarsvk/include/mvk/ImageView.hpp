#pragma once

#include "volk.h"

#include <memory>

#include "mvk/Format.hpp"
#include "mvk/Image.hpp"
#include "mvk/ImageSubresourceRange.hpp"
#include "mvk/ImageViewType.hpp"

namespace mvk {
    class Device;

    class ImageView {
    public:
        struct CreateInfo {
            ImageViewType viewType;
            Format format;
            ImageSubresourceRange subresourceRange;
        };

    private:
        VkImageView _handle;
        const Image * _image;
        CreateInfo _info;

    public:
        void * userData;

        ImageView() :
            _handle(VK_NULL_HANDLE),
            _image(nullptr) {}

        ImageView(const Image * image, const CreateInfo& createInfo);

        ImageView(const Image * image);

        ImageView(const std::unique_ptr<Image>& image) :
            ImageView(image.get()) {}

        ImageView(const ImageView&) = delete;

        ImageView(ImageView&&) = default;

        ~ImageView();

        ImageView& operator= (const ImageView&) = delete;

        ImageView& operator= (ImageView&&) = default;

        inline VkImageView getHandle() const noexcept {
            return _handle;
        }

        inline const Image * getImage() const noexcept {
            return _image;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        Device * getDevice() const;
    };
}