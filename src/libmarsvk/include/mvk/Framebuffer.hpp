#pragma once

#include "volk.h"

#include "mvk/ImageView.hpp"

#include <vector>

namespace mvk {
    class Device;
    class RenderPass;

    class Framebuffer {
    public:
        struct CreateInfo {
            int width;
            int height;
            int layers;
        };

    private:
        VkFramebuffer _handle;
        CreateInfo _info;
        const RenderPass * _renderPass;
        std::vector<const ImageView *> _attachments;

    public:
        void * userData;

        Framebuffer() :
            _handle(VK_NULL_HANDLE),
            _renderPass(nullptr) {}

        Framebuffer(const RenderPass * renderPass, const CreateInfo& createInfo, const std::vector<const ImageView *> attachments);

        Framebuffer(const Framebuffer&) = delete;

        Framebuffer(Framebuffer&&) = default;

        ~Framebuffer();

        Framebuffer& operator= (const Framebuffer&) = delete;

        Framebuffer& operator= (Framebuffer&&) = default;

        inline const ImageView * getAttachment(int index) const noexcept {
            return _attachments[index];
        }

        inline const std::vector<const ImageView *>& getAttachments() const noexcept {
            return _attachments;
        }

        inline const RenderPass * getRenderPass() const noexcept {
            return _renderPass;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        Device * getDevice() const;
    };
}
