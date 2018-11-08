#include "mvk/Buffer.hpp"

#include <cstdint>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <iostream>
#include <vector>

#include "mvk/Device.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/QueueFamily.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        VkMemoryPropertyFlags flags(MemoryUsage memoryUsage) {
            switch (memoryUsage) {
                case MemoryUsage::CPU_ONLY:
                case MemoryUsage::CPU_TO_GPU:
                    return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                case MemoryUsage::GPU_TO_CPU:
                    return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                case MemoryUsage::GPU_ONLY:
                    return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
                default:
                    return 0;
            }
        }

        std::uint32_t getMemoryType(Device * device, std::uint32_t memoryTypeBitsRequirement, MemoryUsage memoryUsage) {
            auto pPhysicalDevice = device->getPhysicalDevice();
            auto memoryProperties = pPhysicalDevice->getMemoryProperties();
            auto requiredProperties = flags(memoryUsage);
            const auto memoryCount = memoryProperties.memoryTypeCount;

            for (std::uint32_t memoryIndex = 0; memoryIndex < memoryCount; ++memoryIndex) {
                const auto memoryTypeBits = static_cast<std::uint32_t> (1u << memoryIndex);
                const bool isRequiredMemoryType = memoryTypeBitsRequirement & memoryTypeBits;
                const auto properties = memoryProperties.memoryTypes[memoryIndex].propertyFlags;
                const bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;

                if (isRequiredMemoryType && hasRequiredProperties) {
                    return memoryIndex;
                }
            }

            throw std::runtime_error("Requirested memory type does not exist!");
        }
    }

    Buffer::Buffer(Device * device, const Buffer::CreateInfo& createInfo, MemoryUsage memoryUsage) {
        _device = device;
        _info = createInfo;

        auto pQueueFamilyIndices = std::vector<std::uint32_t>();
        pQueueFamilyIndices.reserve(createInfo.queueFamilies.size());

        for (auto pQueueFamily : createInfo.queueFamilies) {
            pQueueFamilyIndices.push_back(pQueueFamily->getIndex());
        }

        auto bufferCI = VkBufferCreateInfo {};
        bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.flags = static_cast<VkBufferCreateFlags> (createInfo.flags);
        bufferCI.usage = static_cast<VkBufferUsageFlags> (createInfo.usage);
        bufferCI.pQueueFamilyIndices = pQueueFamilyIndices.data();
        bufferCI.queueFamilyIndexCount = static_cast<std::uint32_t> (pQueueFamilyIndices.size());
        bufferCI.sharingMode = static_cast<VkSharingMode> (createInfo.sharingMode);
        bufferCI.size = static_cast<VkDeviceSize> (createInfo.size);

        if (createInfo.exported) {
            Util::vkAssert(vkCreateBuffer(device->getHandle(), &bufferCI, nullptr, &_handle));

            auto memoryReqs = VkMemoryRequirements {};
            
            vkGetBufferMemoryRequirements(device->getHandle(), _handle, &memoryReqs);

            auto exportMemoryAI = VkExportMemoryAllocateInfo {};
            exportMemoryAI.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO;
            exportMemoryAI.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;

            auto memoryAI = VkMemoryAllocateInfo {};
            memoryAI.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAI.pNext = &exportMemoryAI;
            memoryAI.allocationSize = memoryReqs.size;
            memoryAI.memoryTypeIndex = getMemoryType(device, memoryReqs.memoryTypeBits, memoryUsage);

            Util::vkAssert(vkAllocateMemory(device->getHandle(), &memoryAI, nullptr, &_memory.shared));

            vkBindBufferMemory(device->getHandle(), _handle, _memory.shared, 0);            
        } else {
            auto allocationCI = VmaAllocationCreateInfo {};
            allocationCI.usage = static_cast<VmaMemoryUsage> (memoryUsage);

            Util::vkAssert(vmaCreateBuffer(device->getMemoryAllocator(), &bufferCI, &allocationCI, &_handle, &_memory.local, nullptr));
        }
        
    }

    Buffer::~Buffer() noexcept {
        if (VK_NULL_HANDLE == _handle) {
            return;
        }
    
        if (_info.exported) {
            vkDestroyBuffer(_device->getHandle(), _handle, nullptr);
            vkFreeMemory(_device->getHandle(), _memory.shared, nullptr);            
        } else {
            vmaDestroyBuffer(_device->getMemoryAllocator(), _handle, _memory.local);
        }
    }

    Buffer::Buffer(Buffer&& from) noexcept {
        _device = std::move(from._device);
        _info = std::move(from._info);
        _handle = std::exchange(from._handle, nullptr);

        if (_info.exported) {
            _memory.shared = std::exchange(from._memory.shared, nullptr);
        } else {
            _memory.local = std::exchange(from._memory.local, nullptr);
        }
    }

    Buffer& Buffer::operator= (Buffer&& from) noexcept {
        std::swap(_device, from._device);
        std::swap(_info, from._info);
        std::swap(_handle, from._handle);
        std::swap(_memory, from._memory);
        
        return *this;
    }

    void * Buffer::map() {
        void * pData = nullptr;

        if (_info.exported) {
            vkMapMemory(_device->getHandle(), _memory.shared, 0, _info.size, 0, &pData);
        } else {
            Util::vkAssert(vmaMapMemory(_device->getMemoryAllocator(), _memory.local, &pData));
        }

        return pData;
    }

    void Buffer::unmap() noexcept {
        if (_info.exported) {
            vkUnmapMemory(_device->getHandle(), _memory.shared);
        } else {
            vmaUnmapMemory(_device->getMemoryAllocator(), _memory.local);
        }
    }

    int Buffer::getFd() const {
        if (!_info.exported) {
            throw std::runtime_error("Memory is not exported!");
        }

        auto memoryGetFdInfo = VkMemoryGetFdInfoKHR {};
        memoryGetFdInfo.sType = VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR;
        memoryGetFdInfo.memory = _memory.shared;
        memoryGetFdInfo.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;
         
        int fd;
        Util::vkAssert(vkGetMemoryFdKHR(_device->getHandle(), &memoryGetFdInfo, &fd));

        return fd;
    }
}