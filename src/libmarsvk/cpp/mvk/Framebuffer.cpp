#include "mvk/Framebuffer.hpp"

#include "mvk/Device.hpp"
#include "mvk/RenderPass.hpp"

namespace mvk {
    Framebuffer::Framebuffer(const RenderPass * renderPass, const CreateInfo& createInfo, const std::vector<const ImageView *> attachments) {
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
        framebufferCI.width = static_cast<uint32_t> (createInfo.width);
        framebufferCI.height = static_cast<uint32_t> (createInfo.height);
        framebufferCI.layers = static_cast<uint32_t> (createInfo.layers);

        Util::vkAssert(vkCreateFramebuffer(renderPass->getDevice()->getHandle(), &framebufferCI, nullptr, &_handle));
    }

    Framebuffer::~Framebuffer() {
        vkDestroyFramebuffer(getDevice()->getHandle(), _handle, nullptr);
    }

    Device * Framebuffer::getDevice() const {
        return _renderPass->getDevice();
    }
}
