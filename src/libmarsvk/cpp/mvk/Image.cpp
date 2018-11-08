#include "mvk/Image.hpp"

#include "volk.h"
#include "vk_mem_alloc.h"

#include "mvk/Device.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/QueueFamily.hpp"

#include <stdexcept>

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

        if (createInfo.exported) {
            Util::vkAssert(vkCreateImage(*device, &imageCI, nullptr, &_handle));

            auto memoryReqs = VkMemoryRequirements {};

            vkGetImageMemoryRequirements(*device, _handle, &memoryReqs);

            auto exportMemoryAI = VkExportMemoryAllocateInfo {};
            exportMemoryAI.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO;
            exportMemoryAI.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;

            auto memoryAI = VkMemoryAllocateInfo {};
            memoryAI.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAI.pNext = &exportMemoryAI;
            memoryAI.allocationSize = memoryReqs.size;
            memoryAI.memoryTypeIndex = getMemoryType(device, memoryReqs.memoryTypeBits, memoryUsage);

            Util::vkAssert(vkAllocateMemory(*device, &memoryAI, nullptr, &_memory.shared));
        } else {
            auto allocationCI = VmaAllocationCreateInfo {};
            allocationCI.usage = static_cast<VmaMemoryUsage> (memoryUsage);

            Util::vkAssert(vmaCreateImage(device->getMemoryAllocator(), &imageCI, &allocationCI, &_handle, &_memory.local, nullptr));        
        }

        _external = false;   
    }

    Image::~Image() noexcept {
        if (!_external) {
            if (_info.exported) {
                vkDestroyImage(*_device, _handle, nullptr);
                vkFreeMemory(*_device, _memory.shared, nullptr);
            } else {
                vmaDestroyImage(_device->getMemoryAllocator(), _handle, _memory.local);
            }
        }
    }

    Image::Image(Image&& from) noexcept {
        this->_device = std::move(from._device);
        this->_external = std::move(from._external);
        this->_handle = std::exchange(from._handle, nullptr);
        this->_info = std::move(from._info);
        
        if (_info.exported) {
            this->_memory.shared = std::exchange(from._memory.shared, nullptr);
        } else {
            this->_memory.local = std::exchange(from._memory.local, nullptr);
        }
    }

    Image& Image::operator= (Image&& from) noexcept {
        std::swap(this->_device, from._device);
        std::swap(this->_handle, from._handle);
        std::swap(this->_info, from._info);
        std::swap(this->_memory, from._memory);

        return *this;
    }

    int Image::getFd() const {
        if (!_info.exported) {
            throw std::runtime_error("Memory is not exported!");
        }

        auto memoryGetFdInfo = VkMemoryGetFdInfoKHR {};
        memoryGetFdInfo.sType = VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR;
        memoryGetFdInfo.memory = _memory.shared;
        memoryGetFdInfo.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;

        int fd;
        Util::vkAssert(vkGetMemoryFdKHR(*_device, &memoryGetFdInfo, &fd));

        return fd;
    }
}
