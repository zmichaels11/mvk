#pragma once

#include "volk.h"

#include <vector>

#include "mvk/PipelineStageFlag.hpp"

namespace mvk {
    class CommandBuffer;
    class Device;
    class Fence;
    class QueueFamily;
    class Semaphore;

    class Queue {
    public:
        struct SubmitWaitInfo {
            Semaphore * semaphore;
            PipelineStageFlag stageFlags;
        };

        struct SubmitInfo {
            std::vector<SubmitWaitInfo> waitInfos;
            std::vector<Semaphore *> signalSemaphores;
            std::vector<CommandBuffer *> commandBuffers;
        };

    private:
        VkQueue _handle;
        int _queueIndex;
        QueueFamily * _queueFamily;

    public:
        Queue(QueueFamily * queueFamily, int queueIndex);

        inline QueueFamily * getQueueFamily() const noexcept {
            return _queueFamily;
        }

        Device * getDevice() const;

        void waitIdle();

        void submit(const CommandBuffer * command, Fence * fence = nullptr);
    };
}
