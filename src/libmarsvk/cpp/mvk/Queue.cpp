#include "mvk/Queue.hpp"

#include <vector>

#include "mvk/CommandBuffer.hpp"
#include "mvk/Device.hpp"
#include "mvk/Fence.hpp"
#include "mvk/Image.hpp"
#include "mvk/Semaphore.hpp"
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

    Device * Queue::getDevice() const noexcept {
        return _queueFamily->getDevice();
    }

    void Queue::waitIdle() {
        Util::vkAssert(vkQueueWaitIdle(_handle));
    }

    void Queue::submit(const CommandBuffer * command, const Fence * fence) {
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

    void Queue::submit(const Queue::SubmitInfo& info, const Fence * fence) {
        auto commandBuffers = std::vector<VkCommandBuffer> ();
        commandBuffers.reserve(info.commandBuffers.size());

        for (const auto& commandBuffer : info.commandBuffers) {
            commandBuffers.push_back(commandBuffer->getHandle());
        }

        auto waitSemaphores = std::vector<VkSemaphore> ();
        auto waitDstStageMask = std::vector<VkPipelineStageFlags> ();
        waitSemaphores.reserve(info.waitInfos.size());
        waitDstStageMask.reserve(info.waitInfos.size());

        for (const auto& waitInfo : info.waitInfos) {
            waitSemaphores.push_back(waitInfo.semaphore->getHandle());
            waitDstStageMask.push_back(static_cast<VkPipelineStageFlags> (waitInfo.stageFlags));
        }

        auto signalSemaphores = std::vector<VkSemaphore> ();
        signalSemaphores.reserve(info.signalSemaphores.size());

        for (const auto& signalSemaphore : info.signalSemaphores) {
            signalSemaphores.push_back(signalSemaphore->getHandle());
        }

        auto submitInfo = VkSubmitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = info.waitInfos.size();
        submitInfo.pWaitDstStageMask = waitDstStageMask.data();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();
        submitInfo.commandBufferCount = commandBuffers.size();
        submitInfo.pCommandBuffers = commandBuffers.data();

        VkFence fenceHandle = VK_NULL_HANDLE;

        if (fence) {
            fenceHandle = fence->getHandle();
        }

        Util::vkAssert(vkQueueSubmit(_handle, 1, &submitInfo, fenceHandle));
    }

    Queue::InternalCommandBuffer Queue::acquireCommandBuffer() {
        if (!_commandBuffers.empty()) {
            //NOTE: cannot move icb here since we only want to remove it if the fence is signaled
            auto& icb = _commandBuffers.front();

            if (icb.fence->isSignaled()) {
                icb.fence->reset();
                icb.imageAcquireSemaphore->release();
                icb.presentSemaphore->release();

                auto result = std::move(_commandBuffers.front());

                _commandBuffers.pop();

                return result;
            }
        }

        auto icb = InternalCommandBuffer {};
        icb.commandBuffer = _queueFamily->getCurrentCommandPool()->allocate();
        icb.fence = getDevice()->acquireFence();

        return icb;
    }

    void Queue::present(const Queue::PresentInfo& info, const Swapchain::Backbuffer& backBuffer) {
        auto imageMemoryBarriers = std::vector<ImageMemoryBarrier> ();
        
        if (ImageLayout::TRANSFER_SRC != info.imageLayout) {
            QueueFamily * dstQueueFamily = nullptr;

            if (info.srcQueueFamily || (info.srcQueueFamily && info.srcQueueFamily != _queueFamily)) {
                dstQueueFamily = _queueFamily;
            }

            auto readImageBarrier = ImageMemoryBarrier {};
            readImageBarrier.srcAccessMask = info.imageAccess;
            readImageBarrier.dstAccessMask = AccessFlag::TRANSFER_READ;
            readImageBarrier.oldLayout = info.imageLayout;
            readImageBarrier.newLayout = ImageLayout::TRANSFER_SRC;
            readImageBarrier.srcQueueFamily = info.srcQueueFamily;
            readImageBarrier.dstQueueFamily = dstQueueFamily;
            readImageBarrier.image = info.image;
            readImageBarrier.subresourceRange = info.image->getSubresourceRange(0);

            imageMemoryBarriers.push_back(readImageBarrier);
        }

        {
            auto writeImageBarrier = ImageMemoryBarrier {};
            writeImageBarrier.srcAccessMask = AccessFlag::NONE;
            writeImageBarrier.dstAccessMask = AccessFlag::TRANSFER_WRITE;
            writeImageBarrier.oldLayout = ImageLayout::UNDEFINED;
            writeImageBarrier.newLayout = ImageLayout::TRANSFER_DST;
            writeImageBarrier.image = backBuffer.image;
            writeImageBarrier.subresourceRange = backBuffer.image->getSubresourceRange(0);

            imageMemoryBarriers.push_back(writeImageBarrier);
        }

        auto cmd = acquireCommandBuffer();
        cmd.imageAcquireSemaphore = backBuffer.acquireSemaphore;
        cmd.presentSemaphore = getDevice()->acquireSemaphore();
        cmd.commandBuffer->begin(CommandBufferUsageFlag::ONE_TIME_SUBMIT);
        cmd.commandBuffer->pipelineBarrier(
            info.srcStageMask, PipelineStageFlag::TRANSFER, DependencyFlag::NONE, 
            0, nullptr, 
            0, nullptr, 
            imageMemoryBarriers.size(), imageMemoryBarriers.data());
        
        const auto srcInfo = info.image->getInfo();
        const auto dstInfo = backBuffer.image->getInfo();

        if (srcInfo.extent == dstInfo.extent && srcInfo.format == dstInfo.format) {
            cmd.commandBuffer->copyImage(
                info.image, ImageLayout::TRANSFER_SRC,
                backBuffer.image, ImageLayout::TRANSFER_DST,
                Offset3D {}, Offset3D {}, info.image->getInfo().extent,
                info.image->getSubresourceLayers(0),
                backBuffer.image->getSubresourceLayers(0));
        } else {
            cmd.commandBuffer->blitImage(
                info.image, ImageLayout::TRANSFER_SRC,
                backBuffer.image, ImageLayout::TRANSFER_DST,
                info.image->getSubresourceLayers(0),
                backBuffer.image->getSubresourceLayers(0),
                Filter::LINEAR);
        }

        cmd.commandBuffer->stageImage(backBuffer.image, ImageLayout::TRANSFER_DST, ImageLayout::PRESENT_SRC_KHR, PipelineStageFlag::TRANSFER, PipelineStageFlag::BOTTOM_OF_PIPE, AccessFlag::TRANSFER_WRITE, AccessFlag::MEMORY_READ);
        cmd.commandBuffer->end();

        auto waitInfos = std::vector<SubmitWaitInfo> ();
        waitInfos.reserve(info.waitSemaphores.size() + 1);

        {
            auto copyWaitInfo = SubmitWaitInfo {};
            copyWaitInfo.semaphore = cmd.imageAcquireSemaphore;
            copyWaitInfo.stageFlags = mvk::PipelineStageFlag::BOTTOM_OF_PIPE;

            waitInfos.push_back(copyWaitInfo);
        }

        for (const auto& waitSemaphore : info.waitSemaphores) {
            auto waitInfo = SubmitWaitInfo{};
            waitInfo.semaphore = waitSemaphore;
            waitInfo.stageFlags = info.srcStageMask;

            waitInfos.push_back(waitInfo);
        }

        auto submitInfo = SubmitInfo{};
        submitInfo.commandBuffers.reserve(1);
        submitInfo.commandBuffers.push_back(cmd.commandBuffer.get());
        submitInfo.waitInfos = waitInfos;
        submitInfo.signalSemaphores.reserve(1);
        submitInfo.signalSemaphores.push_back(cmd.presentSemaphore);

        submit(submitInfo, cmd.fence);

        auto imageIndex = static_cast<uint32_t> (backBuffer.index);
        auto swapchainHandle = info.swapchain->getHandle();
        auto waitSemaphoreHandle = cmd.presentSemaphore->getHandle();

        auto presentInfo = VkPresentInfoKHR {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pSwapchains = &swapchainHandle;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &waitSemaphoreHandle;

        vkQueuePresentKHR(_handle, &presentInfo);
        
        _commandBuffers.push(std::move(cmd));
    }
}
