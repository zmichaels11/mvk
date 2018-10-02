#include "mvk/RenderPass.hpp"

#include <vector>

#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        struct Subpass {
            std::vector<VkAttachmentReference> inputAttachments;
            std::vector<VkAttachmentReference> colorAttachments;
            std::vector<VkAttachmentReference> resolveAttachments;
            std::vector<uint32_t> preserveAttachments;
            std::vector<VkAttachmentReference> depthStencilAttachment;
        };
    }

    RenderPass::RenderPass(Device * device, const CreateInfo& createInfo) {
        _device = device;
        _info = createInfo;

        auto attachments = std::vector<VkAttachmentDescription> ();
        attachments.reserve(createInfo.attachments.size());

        for (const auto& attachment : createInfo.attachments) {
            auto vkAttachment = VkAttachmentDescription {};
            vkAttachment.flags = attachment.flags;
            vkAttachment.format = static_cast<VkFormat> (attachment.format);
            vkAttachment.samples = static_cast<VkSampleCountFlagBits> (attachment.samples);
            vkAttachment.loadOp = static_cast<VkAttachmentLoadOp> (attachment.loadOp);
            vkAttachment.storeOp = static_cast<VkAttachmentStoreOp> (attachment.storeOp);
            vkAttachment.stencilLoadOp = static_cast<VkAttachmentLoadOp> (attachment.stencilLoadOp);
            vkAttachment.stencilStoreOp = static_cast<VkAttachmentStoreOp> (attachment.stencilStoreOp);
            vkAttachment.initialLayout = static_cast<VkImageLayout> (attachment.initialLayout);
            vkAttachment.finalLayout = static_cast<VkImageLayout> (attachment.finalLayout);

            attachments.push_back(vkAttachment);
        }

        auto deserialize = [](std::vector<VkAttachmentReference>& dst, const std::vector<AttachmentReference>& src) {
            for (const auto& reference : src) {
                auto vkref = VkAttachmentReference {};
                vkref.attachment = reference.attachment;
                vkref.layout = static_cast<VkImageLayout> (reference.layout);

                dst.push_back(vkref);
            }
        };

        auto subpasses = std::vector<VkSubpassDescription> ();
        auto descriptionInfos = std::vector<Subpass> ();

        for (const auto& subpass : createInfo.subpasses) {
            auto descriptionInfo = Subpass {};

            deserialize(descriptionInfo.inputAttachments, subpass.inputAttachments);
            deserialize(descriptionInfo.colorAttachments, subpass.colorAttachments);
            deserialize(descriptionInfo.resolveAttachments, subpass.resolveAttachments);

            for (const auto& preserve : subpass.preserveAttachments) {
                descriptionInfo.preserveAttachments.push_back(static_cast<uint32_t> (preserve));
            }
            
            if (!subpass.depthStencilAttachment.empty()) {
                auto depthStencil = VkAttachmentReference {};
                depthStencil.attachment = subpass.depthStencilAttachment[0].attachment;
                depthStencil.layout = static_cast<VkImageLayout> (subpass.depthStencilAttachment[0].layout);

                descriptionInfo.depthStencilAttachment.push_back(depthStencil);
            }

            auto description = VkSubpassDescription {};
            description.flags = subpass.flags;
            description.pipelineBindPoint = static_cast<VkPipelineBindPoint> (subpass.pipelineBindpoint);
            description.inputAttachmentCount = descriptionInfo.inputAttachments.size();
            description.pInputAttachments = descriptionInfo.inputAttachments.data();
            description.colorAttachmentCount = descriptionInfo.colorAttachments.size();
            description.pColorAttachments = descriptionInfo.colorAttachments.data();
            description.pResolveAttachments = descriptionInfo.resolveAttachments.data();

            if (descriptionInfo.depthStencilAttachment.empty()) {
                description.pDepthStencilAttachment = nullptr;
            } else {
                description.pDepthStencilAttachment = descriptionInfo.depthStencilAttachment.data();
            }

            description.preserveAttachmentCount = descriptionInfo.preserveAttachments.size();
            description.pPreserveAttachments = descriptionInfo.preserveAttachments.data();

            descriptionInfos.push_back(std::move(descriptionInfo));
            subpasses.push_back(description);
        }

        auto dependencies = std::vector<VkSubpassDependency> ();

        for (const auto& dep : createInfo.dependencies) {
            auto dependency = VkSubpassDependency {};
            dependency.srcSubpass = static_cast<uint32_t> (dep.srcSubpass);
            dependency.dstSubpass = static_cast<uint32_t> (dep.dstSubpass);
            dependency.srcStageMask = static_cast<VkPipelineStageFlags> (dep.srcStageMask);
            dependency.dstStageMask = static_cast<VkPipelineStageFlags> (dep.dstStageMask);
            dependency.srcAccessMask = static_cast<VkAccessFlags> (dep.srcAccessMask);
            dependency.dstAccessMask = static_cast<VkAccessFlags> (dep.dstAccessMask);
            dependency.dependencyFlags = static_cast<VkDependencyFlags> (dep.dependencyFlags);

            dependencies.push_back(dependency);
        }        

        auto renderPassCI = VkRenderPassCreateInfo {};
        renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCI.flags = createInfo.flags;
        renderPassCI.pAttachments = attachments.data();
        renderPassCI.attachmentCount = attachments.size();
        renderPassCI.subpassCount = subpasses.size();
        renderPassCI.pSubpasses = subpasses.data();
        renderPassCI.dependencyCount = dependencies.size();
        renderPassCI.pDependencies = dependencies.data();

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateRenderPass(device->getHandle(), &renderPassCI, nullptr, &_handle));
    }

    RenderPass::~RenderPass() {
        vkDestroyRenderPass(getDevice()->getHandle(), _handle, nullptr);
    }
}
