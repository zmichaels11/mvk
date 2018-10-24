#pragma once

#include "volk.h"

#include <memory>
#include <utility>

#include "mvk/Format.hpp"
#include "mvk/Image.hpp"
#include "mvk/ImageSubresourceRange.hpp"
#include "mvk/ImageViewType.hpp"

namespace mvk {
    class Device;

    //! An ImageView object wraps an Image and defines a section of it for shader Image operations.
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

        ImageView(const ImageView&) = delete;
        ImageView& operator= (const ImageView&) = delete;

    public:
        //! The user-defined point to metadata.
        std::shared_ptr<void> userData;

        //! Creates an empty ImageView.
        ImageView() noexcept:
            _handle(VK_NULL_HANDLE),
            _image(nullptr) {}

        //! Creates an ImageView.
        /*!
            \param image is the Image.
            \param createInfo is the construction parameters.
        */
        ImageView(const Image * image, const CreateInfo& createInfo);

        //! Creates an ImageView.
        /*!
            This function unwraps the unique_ptr and calls the raw-pointer variant.
        */
        ImageView(const std::unique_ptr<Image>& image, const CreateInfo& createInfo) :
            ImageView(image.get(), createInfo) {}

        //! Creates an ImageView and infers the construction parameters.
        /*!
            This function will assume an ImageViewType that aligns with the dimensionality of the source Image.
            If the source Image has array layers, the ImageView will be layered. The Format used will be the
            exact format of the source Image and the entire subresourceRange will be available.

            \param image is the Image.
        */
        ImageView(const Image * image);

        //! Creates an ImageView and infers the construction parameters.
        /*!
            This function unwraps the unique_ptr and calls the raw-pointer variant.
        */
        ImageView(const std::unique_ptr<Image>& image) :
            ImageView(image.get()) {}

        //! Move-constructs the ImageView.
        /*!
            \param from the other ImageView.
        */
        ImageView(ImageView&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _image(std::move(from._image)),
            _info(std::move(from._info)) {}

        //! Deletes the ImageView and releases all resources.
        ~ImageView() noexcept;

        //! Move-assigns the ImageView.
        /*!
            \param from the other ImageView.
        */
        ImageView& operator= (ImageView&& from) noexcept;

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the handle.
        */
        inline VkImageView getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkImageView() const noexcept {
            return _handle;
        }

        //! Retrieves the Image wrapped by this ImageView.
        /*!
            \return the Image.
        */
        inline const Image * getImage() const noexcept {
            return _image;
        }

        //! Retrieves the construction parameters
        /*!
            \return a const reference to the parameters used to construct this ImageView.
        */
        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;
    };
}