#include "mvk/DescriptorSet.hpp"

#include "mvk/Buffer.hpp"
#include "mvk/DescriptorPool.hpp"
#include "mvk/Device.hpp"
#include "mvk/ImageView.hpp"
#include "mvk/Sampler.hpp"

namespace mvk {
    Device * DescriptorSet::getDevice() const noexcept {
        return _pool->getDevice();
    }

    void DescriptorSet::release() {
        _pool->releaseDescriptorSet(this);
    }

    void DescriptorSet::writeBuffer(
        DescriptorType type, int binding, const Buffer * buffer, 
        VkDeviceSize offset, VkDeviceSize range) noexcept {

        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = buffer->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range = range;

        VkWriteDescriptorSet descriptorWrite {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _handle;
        descriptorWrite.dstBinding = static_cast<uint32_t> (binding);
        descriptorWrite.descriptorType = static_cast<VkDescriptorType> (type);
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.descriptorCount = 1;

        vkUpdateDescriptorSets(getDevice()->getHandle(), 1, &descriptorWrite, 0, nullptr);
    }

    void DescriptorSet::writeImage(
        DescriptorType type, int binding, const Sampler * sampler, ImageLayout imageLayout, const ImageView * view) noexcept {

        VkDescriptorImageInfo imageInfo {};
        imageInfo.imageView = view->getHandle();
        imageInfo.imageLayout = static_cast<VkImageLayout> (imageLayout);
        
        if (sampler) {
            imageInfo.sampler = sampler->getHandle();
        }

        VkWriteDescriptorSet descriptorWrite {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _handle;
        descriptorWrite.dstBinding = binding;
        descriptorWrite.descriptorType = static_cast<VkDescriptorType> (type);
        descriptorWrite.pImageInfo = &imageInfo;
        descriptorWrite.descriptorCount = 1;

        vkUpdateDescriptorSets(getDevice()->getHandle(), 1, &descriptorWrite, 0, nullptr);
    }
}
