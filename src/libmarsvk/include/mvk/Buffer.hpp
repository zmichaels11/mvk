#pragma once

#include <cstddef>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <functional>
#include <set>

#include "mvk/BufferUsageFlag.hpp"
#include "mvk/MemoryUsage.hpp"
#include "mvk/SharingMode.hpp"

namespace mvk {
    class Device;
    class QueueFamily;

    class Buffer {
    public:
        struct CreateInfo {
            int flags;
            VkDeviceSize size;
            BufferUsageFlag usage;
            SharingMode sharingMode;
            std::set<QueueFamily * > queueFamilies;
        };

        static constexpr VkDeviceSize WHOLE_SIZE = VK_WHOLE_SIZE;

    private:
        Device * _device;
        VkBuffer _handle;
        CreateInfo _info;
        VmaAllocation _memory;
        
    public:
        void * userDat;

        Buffer() :
            _device(nullptr) {}

        Buffer(Device * device, const CreateInfo& info, MemoryUsage memoryUsage);

        Buffer(const Buffer&) = delete;

        Buffer(Buffer&&) = default;

        ~Buffer();

        Buffer& operator= (const Buffer&) = delete;

        Buffer& operator= (Buffer&&) = default;

        inline VkBuffer getHandle() const {
            return _handle;
        }

        inline Device * getDevice() const {
            return _device;
        }

        inline const CreateInfo& getInfo() const {
            return _info;
        }

        void * map();

        void unmap();

        inline void mapping(std::function<void(void *)> fn) {
            fn(map());
            unmap();
        }

        template<class ptr_t>
        inline void mapping(std::function<void(ptr_t *)> fn) {
            auto pData = reinterpret_cast<ptr_t *> (map());

            fn(pData);
            unmap();
        }
    };
}
