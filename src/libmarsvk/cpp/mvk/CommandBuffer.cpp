#include "mvk/CommandBuffer.hpp"

#include <sstream>
#include <stdexcept>

#include "mvk/Buffer.hpp"
#include "mvk/CommandPool.hpp"
#include "mvk/DescriptorSet.hpp"
#include "mvk/Device.hpp"
#include "mvk/Framebuffer.hpp"
#include "mvk/Image.hpp"
#include "mvk/Pipeline.hpp"
#include "mvk/RenderPass.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    CommandBuffer::~CommandBuffer() {
        vkFreeCommandBuffers(getDevice()->getHandle(), _pool->getHandle(), 1, &_handle);
    }

    Device * CommandBuffer::getDevice() const {
        return _pool->getDevice();
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
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindPoint());
        auto layout = pipeline->getPipelineLayout()->getHandle();
        auto set = descriptorSet->getHandle();

        vkCmdBindDescriptorSets(_handle, bindPoint, layout, static_cast<uint32_t> (firstSet), 1, &set, 0, nullptr);
    }

    void CommandBuffer::bindPipeline(const Pipeline * pipeline) {
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindPoint());

        vkCmdBindPipeline(_handle, bindPoint, pipeline->getHandle());
    }

    void CommandBuffer::dispatch(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ) {
        vkCmdDispatch(_handle, groupsX, groupsY, groupsZ);
    }

    void CommandBuffer::dispatchIndirect(const Buffer * buffer, std::ptrdiff_t offset) {
        vkCmdDispatchIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset));
    }

    void CommandBuffer::beginRenderPass(const Framebuffer * framebuffer, SubpassContents contents) {
        const auto& info = framebuffer->getInfo();
        const auto& attachments = framebuffer->getAttachments();

        auto clearValues = std::vector<VkClearValue> ();
        clearValues.resize(attachments.size());

        for (const auto& attachment : attachments) {
            auto aspect = Util::aspect(attachment->getInfo().format);
            auto clearValue = VkClearValue {};

            if (AspectFlag::COLOR == (aspect & AspectFlag::COLOR)) {
                clearValue.color.float32[0] = 0.0F;
                clearValue.color.float32[1] = 0.0F;
                clearValue.color.float32[2] = 0.0F;
                clearValue.color.float32[3] = 1.0F;
            } else if (AspectFlag::DEPTH_STENCIL == (aspect & AspectFlag::DEPTH_STENCIL)) {
                clearValue.depthStencil.depth = 1.0F;
                clearValue.depthStencil.stencil = 0u;
            }

            clearValues.push_back(clearValue);
        }

        auto renderPassBI = VkRenderPassBeginInfo {};
        renderPassBI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBI.renderPass = framebuffer->getRenderPass()->getHandle();
        renderPassBI.framebuffer = framebuffer->getHandle();
        renderPassBI.renderArea.offset.x = 0;
        renderPassBI.renderArea.offset.y = 0;
        renderPassBI.renderArea.extent.width = info.width;
        renderPassBI.renderArea.extent.height = info.height;
        renderPassBI.clearValueCount = clearValues.size();
        renderPassBI.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(_handle, &renderPassBI, static_cast<VkSubpassContents> (contents));
    }

    void CommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(_handle);
    }

    void CommandBuffer::nextSubpass(SubpassContents contents) {
        vkCmdNextSubpass(_handle, static_cast<VkSubpassContents> (contents));
    }

    void CommandBuffer::setViewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
        auto viewport = VkViewport {};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;

        vkCmdSetViewport(_handle, 0, 1, &viewport);
    }

    void CommandBuffer::setScissor(int x, int y, int width, int height) {
        auto rect = VkRect2D {};
        rect.offset.x = x;
        rect.offset.y = y;
        rect.extent.width = width;
        rect.extent.height = height;

        vkCmdSetScissor(_handle, 0, 1, &rect);
    }

    void CommandBuffer::bindVertexBuffer(int binding, const Buffer * buffer, std::ptrdiff_t offset) {
        auto handle = buffer->getHandle();
        auto off = static_cast<VkDeviceSize> (offset);

        vkCmdBindVertexBuffers(_handle, binding, 1, &handle, &off);
    }

    void CommandBuffer::bindIndexBuffer(const Buffer * buffer, std::ptrdiff_t offset, IndexType indexType) {
        auto off = static_cast<VkDeviceSize> (offset);
        auto type = static_cast<VkIndexType> (indexType);

        vkCmdBindIndexBuffer(_handle, buffer->getHandle(), off, type);
    }

    void CommandBuffer::draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
        vkCmdDraw(_handle, static_cast<uint32_t> (vertexCount), static_cast<uint32_t> (instanceCount), static_cast<uint32_t> (firstVertex), static_cast<uint32_t> (firstInstance));
    }

    void CommandBuffer::drawIndexed(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance) {
        vkCmdDrawIndexed(_handle, static_cast<uint32_t> (indexCount), static_cast<uint32_t> (instanceCount), static_cast<uint32_t> (firstIndex), static_cast<uint32_t> (vertexOffset), static_cast<uint32_t> (firstInstance));
    }

    void CommandBuffer::drawIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) {
        vkCmdDrawIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset), static_cast<uint32_t> (drawCount), static_cast<uint32_t> (stride));
    }

    void CommandBuffer::drawIndexedIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) {
        vkCmdDrawIndexedIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset), static_cast<uint32_t> (drawCount), static_cast<uint32_t> (stride));
    }

    void CommandBuffer::pushConstants(const Pipeline * pipeline, ShaderStage stages, int offset, int size, const void * data) {
        vkCmdPushConstants(_handle, pipeline->getPipelineLayout()->getHandle(), static_cast<VkShaderStageFlags> (stages), static_cast<uint32_t> (offset), static_cast<uint32_t> (size), data);
    }

    void CommandBuffer::copyBufferToImage(
            const Buffer * src, std::ptrdiff_t bufferOffset, 
            const Image * dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent) {

        auto bufferImageCopy = VkBufferImageCopy {};
        bufferImageCopy.bufferOffset = static_cast<VkDeviceSize> (bufferOffset);
        bufferImageCopy.imageOffset.x = offset.x;
        bufferImageCopy.imageOffset.y = offset.y;
        bufferImageCopy.imageOffset.z = offset.z;
        bufferImageCopy.imageExtent.width = extent.width;
        bufferImageCopy.imageExtent.height = extent.height;
        bufferImageCopy.imageExtent.depth = extent.depth;
        bufferImageCopy.imageSubresource.aspectMask = static_cast<VkImageAspectFlags> (subresourceRange.aspectMask);
        bufferImageCopy.imageSubresource.baseArrayLayer = subresourceRange.baseArrayLayer;
        bufferImageCopy.imageSubresource.mipLevel = subresourceRange.mipLevel;
        bufferImageCopy.imageSubresource.layerCount = subresourceRange.layerCount;
        
        vkCmdCopyBufferToImage(_handle, src->getHandle(), dst->getHandle(), static_cast<VkImageLayout> (layout), 1, &bufferImageCopy);
    }

    void CommandBuffer::copyImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource) {

        auto imageCopy = VkImageCopy {};
        imageCopy.srcOffset.x = srcOffset.x;
        imageCopy.srcOffset.y = srcOffset.y;
        imageCopy.srcOffset.z = srcOffset.z;
        imageCopy.dstOffset.x = dstOffset.x;
        imageCopy.dstOffset.y = dstOffset.y;
        imageCopy.dstOffset.z = dstOffset.z;
        imageCopy.extent.width = extent.width;
        imageCopy.extent.height = extent.height;
        imageCopy.extent.depth = extent.depth;
        imageCopy.srcSubresource.aspectMask = static_cast<VkImageAspectFlags> (srcSubresource.aspectMask);
        imageCopy.srcSubresource.baseArrayLayer = srcSubresource.baseArrayLayer;
        imageCopy.srcSubresource.layerCount = srcSubresource.layerCount;
        imageCopy.srcSubresource.mipLevel = srcSubresource.mipLevel;
        imageCopy.dstSubresource.aspectMask = static_cast<VkImageAspectFlags> (dstSubresource.aspectMask);
        imageCopy.dstSubresource.baseArrayLayer = dstSubresource.baseArrayLayer;
        imageCopy.dstSubresource.layerCount = dstSubresource.layerCount;
        imageCopy.dstSubresource.mipLevel = dstSubresource.mipLevel;

        vkCmdCopyImage(_handle, src->getHandle(), static_cast<VkImageLayout> (srcLayout), dst->getHandle(), static_cast<VkImageLayout> (dstLayout), 1, &imageCopy);
    }

    void CommandBuffer::blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter) {

        auto imageBlit = VkImageBlit {};
        imageBlit.srcOffsets[0].x = srcOffset0.x;
        imageBlit.srcOffsets[0].y = srcOffset0.y;
        imageBlit.srcOffsets[0].z = srcOffset0.z;
        imageBlit.srcOffsets[1].x = srcOffset1.x;
        imageBlit.srcOffsets[1].y = srcOffset1.y;
        imageBlit.srcOffsets[1].z = srcOffset1.z;
        imageBlit.dstOffsets[0].x = dstOffset0.x;
        imageBlit.dstOffsets[0].y = dstOffset0.y;
        imageBlit.dstOffsets[0].z = dstOffset0.z;
        imageBlit.dstOffsets[1].x = dstOffset1.x;
        imageBlit.dstOffsets[1].y = dstOffset1.y;
        imageBlit.dstOffsets[1].z = dstOffset1.z;
        imageBlit.srcSubresource.aspectMask = static_cast<VkImageAspectFlags> (srcSubresource.aspectMask);
        imageBlit.srcSubresource.baseArrayLayer = srcSubresource.baseArrayLayer;
        imageBlit.srcSubresource.layerCount = srcSubresource.layerCount;
        imageBlit.srcSubresource.mipLevel = srcSubresource.mipLevel;
        imageBlit.dstSubresource.aspectMask = static_cast<VkImageAspectFlags> (dstSubresource.aspectMask);
        imageBlit.dstSubresource.baseArrayLayer = dstSubresource.baseArrayLayer;
        imageBlit.dstSubresource.layerCount = dstSubresource.layerCount;
        imageBlit.dstSubresource.mipLevel = dstSubresource.mipLevel;

        vkCmdBlitImage(_handle, src->getHandle(), static_cast<VkImageLayout> (srcLayout), dst->getHandle(), static_cast<VkImageLayout> (dstLayout), 1, &imageBlit, static_cast<VkFilter> (filter));
    }

    void CommandBuffer::blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter) {

        auto srcOffset1 = Offset3D{};
        srcOffset1.x = src->getInfo().extent.width;
        srcOffset1.y = src->getInfo().extent.height;
        srcOffset1.z = src->getInfo().extent.depth;

        auto dstOffset1 = Offset3D{};
        dstOffset1.x = dst->getInfo().extent.width;
        dstOffset1.y = dst->getInfo().extent.height;
        dstOffset1.z = dst->getInfo().extent.depth;

        blitImage(src, srcLayout, dst, dstLayout, Offset3D{}, srcOffset1, Offset3D{}, dstOffset1, srcSubresource, dstSubresource, filter);
    }

    void CommandBuffer::stageImage(
            const Image * image, 
            ImageLayout oldLayout, ImageLayout newLayout,
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            AccessFlag srcAccess, AccessFlag dstAccess) {

        auto imageMemoryBarrier = VkImageMemoryBarrier {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags> (srcAccess);
        imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags> (dstAccess);
        imageMemoryBarrier.oldLayout = static_cast<VkImageLayout> (oldLayout);
        imageMemoryBarrier.newLayout = static_cast<VkImageLayout> (newLayout);
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        auto subresourceRange = image->getFullRange();

        imageMemoryBarrier.subresourceRange.aspectMask = static_cast<VkImageAspectFlags> (subresourceRange.aspectMask);
        imageMemoryBarrier.subresourceRange.baseArrayLayer = subresourceRange.baseArrayLayer;
        imageMemoryBarrier.subresourceRange.layerCount = subresourceRange.layerCount;
        imageMemoryBarrier.subresourceRange.baseMipLevel = subresourceRange.baseMipLevel;
        imageMemoryBarrier.subresourceRange.levelCount = subresourceRange.levelCount;

        vkCmdPipelineBarrier(_handle, static_cast<VkPipelineStageFlags> (srcStageMask), static_cast<VkPipelineStageFlags> (dstStageMask), 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    }

    void CommandBuffer::copyBuffer(
            const Buffer * src, const Buffer * dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size) {

        auto region = VkBufferCopy {};
        region.srcOffset = static_cast<VkDeviceSize> (srcOffset);
        region.dstOffset = static_cast<VkDeviceSize> (dstOffset);
        region.size = static_cast<VkDeviceSize> (size);

        vkCmdCopyBuffer(_handle, src->getHandle(), dst->getHandle(), 1, &region);
    }
}
