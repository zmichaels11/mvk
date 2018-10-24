#pragma once

#include "volk.h"

#include <cstddef>

#include "mvk/ImageView.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace mvk {
    class Device;
    class RenderPass;

    //! A Framebuffer object represents a collection of Image objects to render to.
    class Framebuffer {
    public:
        //! Construction parameters for a Framebuffer object.
        struct CreateInfo {
            int width;      /*!< The width of the Framebuffer object. */
            int height;     /*!< The height of the Framebuffer object. */
            int layers;     /*!< The number of layers in the Framebuffer object. */
        };

    private:
        VkFramebuffer _handle;
        CreateInfo _info;
        const RenderPass * _renderPass;
        std::vector<const ImageView *> _attachments;

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator= (const Framebuffer&) = delete;

    public:
        //! User specified metadata.
        std::shared_ptr<void> userData;

        //! Creates an empty Framebuffer object.
        Framebuffer() noexcept:
            _handle(VK_NULL_HANDLE),
            _renderPass(nullptr) {}

        //! Creates a Framebuffer object.
        /*!
            \param renderPass is a RenderPass that the Framebuffer will be used with.
            \param createInfo is the construction parameters
            \param attachments is a list of pointers to ImageView objects to use as Framebuffer attachments.
        */
        Framebuffer(const RenderPass * renderPass, const CreateInfo& createInfo, const std::vector<const ImageView *>& attachments);

        //! Creates a Framebuffer object.
        /*!
            This function unwraps the unique_ptr for RenderPass and chains the raw-pointer variant.
        */
        Framebuffer(const std::unique_ptr<RenderPass>& renderPass, const CreateInfo& createInfo, const std::vector<const ImageView *>& attachments) :
            Framebuffer(renderPass.get(), createInfo, attachments) {}

        //! Move-constructs a Framebuffer object.
        /*!
            \param from the other Framebuffer.
        */
        Framebuffer(Framebuffer&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _info(std::move(from._info)),
            _renderPass(std::move(from._renderPass)),
            _attachments(std::move(from._attachments)) {}

        //! Deconstructs the Framebuffer object and releases all resources.
        ~Framebuffer() noexcept;

        //! Move-assigns a Framebuffer object.
        /*!
            \param from the other Framebuffer.
        */
        Framebuffer& operator= (Framebuffer&& from) noexcept;

        //! Retrieves the underlying Framebuffer handle.
        /*!
            \return the handle.
        */
        inline VkFramebuffer getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Framebuffer object.
        inline operator VkFramebuffer() const noexcept {
            return _handle;
        }

        //! Retrieves an attachment.
        /*!
            \param index is the attachment index.
            \return the ImageView for the attachment.
        */
        inline const ImageView * getAttachment(std::ptrdiff_t index) const noexcept {
            return _attachments[index];
        }

        //! Retrieves a list of all attachments.
        /*!
            \return all attachments.
        */
        inline const std::vector<const ImageView *>& getAttachments() const noexcept {
            return _attachments;
        }

        //! Retrieves the RenderPass.
        /*!
            \return the RenderPass that this Framebuffer can be used in.
        */
        inline const RenderPass * getRenderPass() const noexcept {
            return _renderPass;
        }

        //! Retrieves a const reference to the construction parameters.
        /*!
            \return a const references to an immutable copy of the construction parameters.
        */
        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        //! Retrieves the device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;
    };
}
