#include "mvk/Image.hpp"

#include "volk.h"
#include "vk_mem_alloc.h"

#include "mvk/Device.hpp"
#include "mvk/QueueFamily.hpp"

namespace mvk {
    Image::Image(Device * device, const Image::CreateInfo& createInfo, MemoryUsage memoryUsage) {
        _device = device;
        _info = createInfo;

        auto queueFamilyIndicies = std::vector<uint32_t> ();

        for (const auto& queueFamily : createInfo.queueFamilies) {
            queueFamilyIndicies.push_back(queueFamily->getIndex());
        }

        auto imageCI = VkImageCreateInfo {};
        imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCI.flags = createInfo.flags;
        imageCI.imageType = static_cast<VkImageType> (createInfo.imageType);
        imageCI.format = static_cast<VkFormat> (createInfo.format);
        imageCI.mipLevels = createInfo.mipLevels;
        imageCI.arrayLayers = createInfo.arrayLayers;
        imageCI.samples = static_cast<VkSampleCountFlagBits> (createInfo.samples);
        imageCI.tiling = static_cast<VkImageTiling> (createInfo.tiling);
        imageCI.usage = static_cast<VkImageUsageFlags> (createInfo.usage);
        imageCI.sharingMode = static_cast<VkSharingMode> (createInfo.sharingMode);
        imageCI.pQueueFamilyIndices = queueFamilyIndicies.data();
        imageCI.queueFamilyIndexCount = queueFamilyIndicies.size();
        imageCI.initialLayout = static_cast<VkImageLayout> (createInfo.initialLayout);
        imageCI.extent.width = createInfo.extent.width;
        imageCI.extent.height = createInfo.extent.height;
        imageCI.extent.depth = createInfo.extent.depth;

        auto allocationCI = VmaAllocationCreateInfo {};
        allocationCI.usage = static_cast<VmaMemoryUsage> (memoryUsage);

        _handle = VK_NULL_HANDLE;
        _memory = VK_NULL_HANDLE;

        Util::vkAssert(vmaCreateImage(device->getMemoryAllocator(), &imageCI, &allocationCI, &_handle, &_memory, nullptr));        
    }

    Image::~Image() {
        if (VK_NULL_HANDLE != _memory) {
            vmaDestroyImage(getDevice()->getMemoryAllocator(), _handle, _memory);
        }
    }

    void * Image::map() {
        void * pData = nullptr;

        vmaMapMemory(getDevice()->getMemoryAllocator(), _memory, &pData);

        return pData;
    }

    void Image::unmap() {
        vmaUnmapMemory(getDevice()->getMemoryAllocator(), _memory);
    }
}
