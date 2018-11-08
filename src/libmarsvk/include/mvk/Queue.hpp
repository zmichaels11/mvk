#pragma once

#include "volk.h"

#include <memory>
#include <queue>
#include <utility>
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

        Queue(const Queue&) = delete;
        Queue& operator= (const Queue&) = delete;

    public:
        Queue() noexcept:
            _handle(VK_NULL_HANDLE),
            _queueIndex(-1),
            _queueFamily(nullptr) {}
            
        Queue(QueueFamily * queueFamily, int queueIndex);

        Queue(Queue&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _queueIndex(std::move(from._queueIndex)),
            _queueFamily(std::move(from._queueFamily)),
            _commandBuffers(std::move(from._commandBuffers)) {}

        Queue& operator= (Queue&&) = default;

        inline QueueFamily * getQueueFamily() const noexcept {
            return _queueFamily;
        }

        inline int getIndex() const noexcept {
            return _queueIndex;
        }

        Device * getDevice() const noexcept;

        void waitIdle();

        void submit(const CommandBuffer * command, const Fence * fence = nullptr);

        inline void submit(const std::unique_ptr<CommandBuffer>& command, const Fence * fence = nullptr) {
            submit(command.get(), fence);
        }

        void submit(const SubmitInfo& submitInfo, const Fence * fence = nullptr);

        void present(const PresentInfo& presentInfo, const Swapchain::Backbuffer& backBuffer);

        inline void present(const PresentInfo& presentInfo) {
            present(presentInfo, presentInfo.swapchain->acquireNextImage());
        }
    };
}
