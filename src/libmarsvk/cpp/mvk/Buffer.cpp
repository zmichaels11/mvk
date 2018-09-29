#include "mvk/Buffer.hpp"

#include <cstdint>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <vector>

#include "mvk/Device.hpp"
#include "mvk/QueueFamily.hpp"

namespace mvk {
    Buffer::Buffer(Device * device, const Buffer::CreateInfo& createInfo, MemoryUsage memoryUsage) {
        _device = device;
        _info = createInfo;

        auto pQueueFamilyIndices = std::vector<std::uint32_t>();
        pQueueFamilyIndices.reserve(createInfo.queueFamilies.size());

        for (auto pQueueFamily : createInfo.queueFamilies) {
            pQueueFamilyIndices.push_back(pQueueFamily->getIndex());
        }

        VkBufferCreateInfo bufferCI {};
        bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.flags = createInfo.flags;
        bufferCI.usage = static_cast<VkBufferUsageFlags> (createInfo.usage);
        bufferCI.pQueueFamilyIndices = pQueueFamilyIndices.data();
        bufferCI.queueFamilyIndexCount = pQueueFamilyIndices.size();
        bufferCI.sharingMode = static_cast<VkSharingMode> (createInfo.sharingMode);
        bufferCI.size = createInfo.size;

        VmaAllocationCreateInfo allocationCI {};
        allocationCI.usage = static_cast<VmaMemoryUsage> (memoryUsage);
        
        vmaCreateBuffer(device->getMemoryAllocator(), &bufferCI, &allocationCI, &_handle, &_memory, nullptr);
    }

    Buffer::~Buffer() {
        if (VK_NULL_HANDLE == _handle) {
            return;
        }

        vmaDestroyBuffer(_device->getMemoryAllocator(), _handle, _memory);
    }

    void * Buffer::map() {
        void * pData = nullptr;

        vmaMapMemory(_device->getMemoryAllocator(), _memory, &pData);

        return pData;
    }

    void Buffer::unmap() {
        vmaUnmapMemory(_device->getMemoryAllocator(), _memory);
    }
}