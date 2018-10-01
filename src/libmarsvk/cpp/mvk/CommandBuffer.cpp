#include "mvk/CommandBuffer.hpp"

#include "mvk/CommandPool.hpp"
#include "mvk/DescriptorSet.hpp"
#include "mvk/Device.hpp"
#include "mvk/Pipeline.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    CommandBuffer::~CommandBuffer() {
        vkFreeCommandBuffers(getDevice()->getHandle(), _pool->getHandle(), 1, &_handle);
    }

    void CommandBuffer::begin(CommandBufferUsageFlag flags) {
        auto commandBufferBI = VkCommandBufferBeginInfo {};
        commandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBI.flags = static_cast<VkCommandBufferUsageFlags> (flags);

        Util::vkAssert(vkBeginCommandBuffer(_handle, &commandBufferBI));
    }

    void CommandBuffer::end() {
        Util::vkAssert(vkEndCommandBuffer(_handle));
    }

    void CommandBuffer::bindDescriptorSet(const Pipeline * pipeline, int firstSet, const DescriptorSet * descriptorSet) {
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindpoint());
        auto layout = pipeline->getPipelineLayout()->getHandle();
        auto set = descriptorSet->getHandle();

        vkCmdBindDescriptorSets(_handle, bindPoint, layout, static_cast<uint32_t> (firstSet), 1, &set, 0, nullptr);
    }

    void CommandBuffer::bindPipeline(const Pipeline * pipeline) {
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindpoint());

        vkCmdBindPipeline(_handle, bindPoint, pipeline->getHandle());
    }

    void CommandBuffer::dispatch(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ) {
        vkCmdDispatch(_handle, groupsX, groupsY, groupsZ);
    }
}
