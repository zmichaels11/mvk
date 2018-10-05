#include "mvk/CommandPool.hpp"

#include <iostream>
#include <stdexcept>

#include "mvk/Device.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    CommandPool::CommandPool(const QueueFamily * queueFamily, CommandPoolCreateFlag flags) {
        _flags = flags;
        _queueFamily = queueFamily;

        auto pDevice = queueFamily->getDevice();

        auto commandPoolCI = VkCommandPoolCreateInfo {};
        commandPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCI.flags = static_cast<VkCommandPoolCreateFlags> (flags);
        commandPoolCI.queueFamilyIndex = static_cast<uint32_t> (queueFamily->getIndex());

        _handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateCommandPool(pDevice->getHandle(), &commandPoolCI, nullptr, &_handle));
    }

    CommandPool::~CommandPool() {
        std::cout << "Freeing CommandPool!" << std::endl;
        vkDestroyCommandPool(getDevice()->getHandle(), _handle, nullptr);
    }

    Device * CommandPool::getDevice() const {
        if (_queueFamily == nullptr) {
            throw std::runtime_error("CommandPool is not valid!");
        }

        return _queueFamily->getDevice();
    }

    void CommandPool::reset(unsigned int flags) {
        vkResetCommandPool(getDevice()->getHandle(), _handle, flags);
    }

    std::unique_ptr<CommandBuffer> CommandPool::allocate(CommandBufferLevel level) {
        auto commandBufferAI = VkCommandBufferAllocateInfo {};
        commandBufferAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAI.commandPool = _handle;
        commandBufferAI.level = static_cast<VkCommandBufferLevel> (level);
        commandBufferAI.commandBufferCount = 1;

        VkCommandBuffer handle = VK_NULL_HANDLE;

        Util::vkAssert(vkAllocateCommandBuffers(getDevice()->getHandle(), &commandBufferAI, &handle));

        return std::make_unique<CommandBuffer> (this, level, handle);
    }
}
