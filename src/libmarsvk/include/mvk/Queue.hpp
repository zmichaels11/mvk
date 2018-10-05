#pragma once

#include "volk.h"

#include <queue>
#include <vector>

#include "mvk/AccessFlag.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/PipelineStageFlag.hpp"
#include "mvk/Swapchain.hpp"

namespace mvk {
    class CommandBuffer;
    class Device;
    class Fence;
    class Image;
    class QueueFamily;
    class Semaphore;

    class Queue {
    public:
        struct SubmitWaitInfo {
            const Semaphore * semaphore;
            PipelineStageFlag stageFlags;
        };

        struct SubmitInfo {
            std::vector<SubmitWaitInfo> waitInfos;
            std::vector<const Semaphore *> signalSemaphores;
            std::vector<const CommandBuffer *> commandBuffers;
        };

        struct PresentInfo {
            Swapchain * swapchain;
            std::vector<const Semaphore *> waitSemaphores;
            const Image * image;
            ImageLayout imageLayout;
            AccessFlag imageAccess;
            const QueueFamily * srcQueueFamily;
            PipelineStageFlag srcStageMask;
        };

    private:
        struct InternalCommandBuffer {
            std::unique_ptr<CommandBuffer> commandBuffer;
            Fence * fence;
            Semaphore * imageAcquireSemaphore;
            Semaphore * presentSemaphore;
        };

        VkQueue _handle;
        int _queueIndex;
        QueueFamily * _queueFamily;
        std::queue<InternalCommandBuffer> _commandBuffers;

        InternalCommandBuffer acquireCommandBuffer();

    public:
        Queue(QueueFamily * queueFamily, int queueIndex);

        Queue(const Queue&) = delete;

        Queue(Queue&&) = default;

        Queue& operator= (const Queue&) = delete;

        Queue& operator= (Queue&&) = default;

        inline QueueFamily * getQueueFamily() const noexcept {
            return _queueFamily;
        }

        Device * getDevice() const;

        void waitIdle();

        void submit(const CommandBuffer * command, const Fence * fence = nullptr);

        void submit(const SubmitInfo& submitInfo, const Fence * fence = nullptr);

        void present(const PresentInfo& presentInfo, const Swapchain::Backbuffer& backBuffer);

        inline void present(const PresentInfo& presentInfo) {
            present(presentInfo, presentInfo.swapchain->acquireNextImage());
        }
    };
}
