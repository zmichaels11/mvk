#include "mvk/DescriptorSet.hpp"

#include "mvk/Buffer.hpp"
#include "mvk/DescriptorPool.hpp"
#include "mvk/Device.hpp"

namespace mvk {
    Device * DescriptorSet::getDevice() const {
        return _pool->getDevice();
    }

    void DescriptorSet::release() {
        _pool->releaseDescriptorSet(this);
    }

    void DescriptorSet::writeBuffer(
        DescriptorType type, int binding, const Buffer * buffer, 
        VkDeviceSize offset, VkDeviceSize range) {

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
}
