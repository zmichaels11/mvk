#pragma once

#include "volk.h"

#include <memory>
#include <utility>
#include <vector>

#include "mvk/AttachmentDescription.hpp"
#include "mvk/SubpassContents.hpp"
#include "mvk/SubpassDependency.hpp"
#include "mvk/SubpassDescription.hpp"

namespace mvk {
    class Device;

    class RenderPass;

    using UPtrRenderPass = std::unique_ptr<RenderPass>;

    class RenderPass {
    public:
        struct CreateInfo {
            unsigned int flags;
            std::vector<AttachmentDescription> attachments;
            std::vector<SubpassDescription> subpasses;
            std::vector<SubpassDependency> dependencies;
        };

        static inline UPtrRenderPass unique_null() noexcept {
            return std::unique_ptr<RenderPass>();
        }

    private:
        VkRenderPass _handle;
        CreateInfo _info;
        Device * _device;

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator= (const RenderPass&) = delete;

    public:
        RenderPass() noexcept:
            _handle(VK_NULL_HANDLE),
            _device(nullptr) {}

        RenderPass(Device * device, const CreateInfo& createInfo);

        RenderPass(RenderPass&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _info(std::move(from._info)),
            _device(std::move(from._device)) {}

        ~RenderPass() noexcept;

        RenderPass& operator= (RenderPass&& from) noexcept;

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
