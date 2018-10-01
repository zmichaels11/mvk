#include "mvk/Queue.hpp"

#include "mvk/CommandBuffer.hpp"
#include "mvk/Device.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    Queue::Queue(QueueFamily * queueFamily, int queueIndex) {
        _queueFamily = queueFamily;
        _queueIndex = queueIndex;

        auto pDevice = queueFamily->getDevice();

        _handle = VK_NULL_HANDLE;

        vkGetDeviceQueue(pDevice->getHandle(), queueFamily->getIndex(), queueIndex, &_handle);
    }

    Device * Queue::getDevice() const {
        return _queueFamily->getDevice();
    }

    void Queue::waitIdle() {
        Util::vkAssert(vkQueueWaitIdle(_handle));
    }

    void Queue::submit(const CommandBuffer * command, Fence * fence) {
        auto cmdHandle = command->getHandle();

        auto submitInfo = VkSubmitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pCommandBuffers = &cmdHandle;
        submitInfo.commandBufferCount = 1;
        
        if (nullptr == fence) {
            Util::vkAssert(vkQueueSubmit(_handle, 1, &submitInfo, VK_NULL_HANDLE));
        } else {
            Util::vkAssert(vkQueueSubmit(_handle, 1, &submitInfo, fence->getHandle()));
        }
    }
}
