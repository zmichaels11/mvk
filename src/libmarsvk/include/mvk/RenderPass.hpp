#pragma once

#include "volk.h"

#include <vector>

#include "mvk/AttachmentDescription.hpp"
#include "mvk/SubpassContents.hpp"
#include "mvk/SubpassDependency.hpp"
#include "mvk/SubpassDescription.hpp"

namespace mvk {
    class Device;

    class RenderPass {
    public:
        struct CreateInfo {
            unsigned int flags;
            std::vector<AttachmentDescription> attachments;
            std::vector<SubpassDescription> subpasses;
            std::vector<SubpassDependency> dependencies;
        };

    private:
        VkRenderPass _handle;
        CreateInfo _info;
        Device * _device;

    public:
        RenderPass() :
            _handle(VK_NULL_HANDLE),
            _device(nullptr) {}

        RenderPass(Device * device, const CreateInfo& createInfo);

        RenderPass(const RenderPass&) = delete;

        RenderPass(RenderPass&&) = default;

        ~RenderPass();

        RenderPass& operator= (const RenderPass&) = delete;

        RenderPass& operator= (RenderPass&&) = default;

        inline Device * getDevice() const noexcept {
            return _device;
        }

        inline VkRenderPass getHandle() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }
    };
}
