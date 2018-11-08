#include "mvk/Framebuffer.hpp"

#include <cstddef>

#include "mvk/Device.hpp"
#include "mvk/RenderPass.hpp"

namespace mvk {
    Framebuffer::Framebuffer(const RenderPass * renderPass, const CreateInfo& createInfo, const std::vector<const ImageView *>& attachments) {
        _renderPass = renderPass;
        _info = createInfo;
        _attachments = attachments;

        auto vkAttachments = std::vector<VkImageView> ();
        vkAttachments.reserve(attachments.size());

        for (const auto& attachment : attachments) {
            vkAttachments.push_back(attachment->getHandle());
        }

        auto framebufferCI = VkFramebufferCreateInfo {};
        framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCI.renderPass = renderPass->getHandle();
        framebufferCI.attachmentCount = vkAttachments.size();
        framebufferCI.pAttachments = vkAttachments.data();
        framebufferCI.width = static_cast<std::uint32_t> (createInfo.width);
        framebufferCI.height = static_cast<std::uint32_t> (createInfo.height);
        framebufferCI.layers = static_cast<std::uint32_t> (createInfo.layers);

        Util::vkAssert(vkCreateFramebuffer(renderPass->getDevice()->getHandle(), &framebufferCI, nullptr, &_handle));
    }

    Framebuffer::~Framebuffer() noexcept {
        vkDestroyFramebuffer(getDevice()->getHandle(), _handle, nullptr);
    }

    Framebuffer& Framebuffer::operator= (Framebuffer&& from) noexcept {
        std::swap(this->_attachments, from._attachments);
        std::swap(this->_handle, from._handle);
        std::swap(this->_info, from._info);
        std::swap(this->_renderPass, from._renderPass);

        return *this;
    }

    Device * Framebuffer::getDevice() const noexcept {
        return _renderPass->getDevice();
    }
}
