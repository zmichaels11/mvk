#include "mvk/CommandBuffer.hpp"

#include <algorithm>
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
    CommandBuffer& CommandBuffer::operator= (CommandBuffer&& from) noexcept {
        std::swap(_pool, from._pool);
        std::swap(_level, from._level);
        std::swap(_handle, from._handle);

        return *this;
    }

    CommandBuffer::~CommandBuffer() noexcept {
        vkFreeCommandBuffers(getDevice()->getHandle(), _pool->getHandle(), 1, &_handle);
    }

    Device * CommandBuffer::getDevice() const noexcept {
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

    void CommandBuffer::bindDescriptorSet(
        const Pipeline * pipeline, int firstSet, const DescriptorSet * descriptorSet, 
        std::size_t nDynamicOffsets, const int * pDynamicOffsets) noexcept {

        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindPoint());
        auto layout = pipeline->getPipelineLayout()->getHandle();
        auto set = descriptorSet->getHandle();
        auto offsets = reinterpret_cast<const uint32_t * > (pDynamicOffsets);

        vkCmdBindDescriptorSets(_handle, bindPoint, layout, static_cast<uint32_t> (firstSet), 1, &set, nDynamicOffsets, offsets);
    }

    void CommandBuffer::bindDescriptorSet(const Pipeline * pipeline, int firstSet, const DescriptorSet * descriptorSet) noexcept {
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindPoint());
        auto layout = pipeline->getPipelineLayout()->getHandle();
        auto set = descriptorSet->getHandle();

        vkCmdBindDescriptorSets(_handle, bindPoint, layout, static_cast<uint32_t> (firstSet), 1, &set, 0, nullptr);
    }

    void CommandBuffer::bindPipeline(const Pipeline * pipeline) noexcept {
        auto bindPoint = static_cast<VkPipelineBindPoint> (pipeline->getBindPoint());

        vkCmdBindPipeline(_handle, bindPoint, pipeline->getHandle());
    }

    void CommandBuffer::dispatch(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ) noexcept {
        vkCmdDispatch(_handle, groupsX, groupsY, groupsZ);
    }

    void CommandBuffer::dispatchIndirect(const Buffer * buffer, std::ptrdiff_t offset) noexcept {
        vkCmdDispatchIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset));
    }

    void CommandBuffer::beginRenderPass(const Framebuffer * framebuffer, SubpassContents contents) noexcept {
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
        renderPassBI.renderArea.extent.width = static_cast<std::uint32_t> (info.width);
        renderPassBI.renderArea.extent.height = static_cast<std::uint32_t> (info.height);
        renderPassBI.clearValueCount = clearValues.size();
        renderPassBI.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(_handle, &renderPassBI, static_cast<VkSubpassContents> (contents));
    }

    void CommandBuffer::endRenderPass() noexcept {
        vkCmdEndRenderPass(_handle);
    }

    void CommandBuffer::nextSubpass(SubpassContents contents) noexcept {
        vkCmdNextSubpass(_handle, static_cast<VkSubpassContents> (contents));
    }

    void CommandBuffer::setViewport(float x, float y, float width, float height, float minDepth, float maxDepth) noexcept {
        auto viewport = VkViewport {};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;

        vkCmdSetViewport(_handle, 0, 1, &viewport);
    }

    void CommandBuffer::setScissor(int x, int y, int width, int height) noexcept {
        auto rect = VkRect2D {};
        rect.offset.x = x;
        rect.offset.y = y;
        rect.extent.width = static_cast<std::uint32_t> (width);
        rect.extent.height = static_cast<std::uint32_t> (height);

        vkCmdSetScissor(_handle, 0, 1, &rect);
    }

    void CommandBuffer::bindVertexBuffer(int binding, const Buffer * buffer, std::ptrdiff_t offset) noexcept {
        auto handle = buffer->getHandle();
        auto off = static_cast<VkDeviceSize> (offset);

        vkCmdBindVertexBuffers(_handle, binding, 1, &handle, &off);
    }

    void CommandBuffer::bindIndexBuffer(const Buffer * buffer, std::ptrdiff_t offset, IndexType indexType) noexcept {
        auto off = static_cast<VkDeviceSize> (offset);
        auto type = static_cast<VkIndexType> (indexType);

        vkCmdBindIndexBuffer(_handle, buffer->getHandle(), off, type);
    }

    void CommandBuffer::draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) noexcept {
        vkCmdDraw(_handle, static_cast<uint32_t> (vertexCount), static_cast<uint32_t> (instanceCount), static_cast<uint32_t> (firstVertex), static_cast<uint32_t> (firstInstance));
    }

    void CommandBuffer::drawIndexed(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance) noexcept {
        vkCmdDrawIndexed(_handle, static_cast<uint32_t> (indexCount), static_cast<uint32_t> (instanceCount), static_cast<uint32_t> (firstIndex), static_cast<uint32_t> (vertexOffset), static_cast<uint32_t> (firstInstance));
    }

    void CommandBuffer::drawIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept {
        vkCmdDrawIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset), static_cast<uint32_t> (drawCount), static_cast<uint32_t> (stride));
    }

    void CommandBuffer::drawIndexedIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept {
        vkCmdDrawIndexedIndirect(_handle, buffer->getHandle(), static_cast<VkDeviceSize> (offset), static_cast<uint32_t> (drawCount), static_cast<uint32_t> (stride));
    }

    void CommandBuffer::pushConstants(const Pipeline * pipeline, ShaderStage stages, std::ptrdiff_t offset, std::size_t size, const void * data) noexcept {
        vkCmdPushConstants(
            _handle, 
            pipeline->getPipelineLayout()->getHandle(), static_cast<VkShaderStageFlags> (stages), 
            static_cast<uint32_t> (offset), static_cast<uint32_t> (size), data);
    }

    void CommandBuffer::copyBufferToImage(
            const Buffer * src, std::ptrdiff_t bufferOffset, 
            const Image * dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent) noexcept {

        auto bufferImageCopy = VkBufferImageCopy {};
        bufferImageCopy.bufferOffset = static_cast<VkDeviceSize> (bufferOffset);
        bufferImageCopy.imageOffset.x = offset.x;
        bufferImageCopy.imageOffset.y = offset.y;
        bufferImageCopy.imageOffset.z = offset.z;
        bufferImageCopy.imageExtent.width = static_cast<std::uint32_t> (extent.width);
        bufferImageCopy.imageExtent.height = static_cast<std::uint32_t> (extent.height);
        bufferImageCopy.imageExtent.depth = static_cast<std::uint32_t> (extent.depth);
        bufferImageCopy.imageSubresource.aspectMask = static_cast<VkImageAspectFlags> (subresourceRange.aspectMask);
        bufferImageCopy.imageSubresource.baseArrayLayer = static_cast<std::uint32_t> (subresourceRange.baseArrayLayer);
        bufferImageCopy.imageSubresource.mipLevel = static_cast<std::uint32_t> (subresourceRange.mipLevel);
        bufferImageCopy.imageSubresource.layerCount = static_cast<std::uint32_t> (subresourceRange.layerCount);
        
        vkCmdCopyBufferToImage(_handle, src->getHandle(), dst->getHandle(), static_cast<VkImageLayout> (layout), 1, &bufferImageCopy);
    }

    void CommandBuffer::copyImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource) noexcept {

        auto imageCopy = VkImageCopy {};
        imageCopy.srcOffset.x = srcOffset.x;
        imageCopy.srcOffset.y = srcOffset.y;
        imageCopy.srcOffset.z = srcOffset.z;
        imageCopy.dstOffset.x = dstOffset.x;
        imageCopy.dstOffset.y = dstOffset.y;
        imageCopy.dstOffset.z = dstOffset.z;
        imageCopy.extent.width = static_cast<std::uint32_t> (extent.width);
        imageCopy.extent.height = static_cast<std::uint32_t> (extent.height);
        imageCopy.extent.depth = static_cast<std::uint32_t> (extent.depth);
        imageCopy.srcSubresource.aspectMask = static_cast<VkImageAspectFlags> (srcSubresource.aspectMask);
        imageCopy.srcSubresource.baseArrayLayer = static_cast<std::uint32_t> (srcSubresource.baseArrayLayer);
        imageCopy.srcSubresource.layerCount = static_cast<std::uint32_t> (srcSubresource.layerCount);
        imageCopy.srcSubresource.mipLevel = static_cast<std::uint32_t> (srcSubresource.mipLevel);
        imageCopy.dstSubresource.aspectMask = static_cast<VkImageAspectFlags> (dstSubresource.aspectMask);
        imageCopy.dstSubresource.baseArrayLayer = static_cast<std::uint32_t> (dstSubresource.baseArrayLayer);
        imageCopy.dstSubresource.layerCount = static_cast<std::uint32_t> (dstSubresource.layerCount);
        imageCopy.dstSubresource.mipLevel = static_cast<std::uint32_t> (dstSubresource.mipLevel);

        vkCmdCopyImage(_handle, src->getHandle(), static_cast<VkImageLayout> (srcLayout), dst->getHandle(), static_cast<VkImageLayout> (dstLayout), 1, &imageCopy);
    }

    void CommandBuffer::blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter) noexcept {

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
        imageBlit.srcSubresource.baseArrayLayer = static_cast<std::uint32_t> (srcSubresource.baseArrayLayer);
        imageBlit.srcSubresource.layerCount = static_cast<std::uint32_t> (srcSubresource.layerCount);
        imageBlit.srcSubresource.mipLevel = static_cast<std::uint32_t> (srcSubresource.mipLevel);
        imageBlit.dstSubresource.aspectMask = static_cast<VkImageAspectFlags> (dstSubresource.aspectMask);
        imageBlit.dstSubresource.baseArrayLayer = static_cast<std::uint32_t> (dstSubresource.baseArrayLayer);
        imageBlit.dstSubresource.layerCount = static_cast<std::uint32_t> (dstSubresource.layerCount);
        imageBlit.dstSubresource.mipLevel = static_cast<std::uint32_t> (dstSubresource.mipLevel);

        vkCmdBlitImage(_handle, src->getHandle(), static_cast<VkImageLayout> (srcLayout), dst->getHandle(), static_cast<VkImageLayout> (dstLayout), 1, &imageBlit, static_cast<VkFilter> (filter));
    }

    void CommandBuffer::blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter) noexcept {

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
            AccessFlag srcAccess, AccessFlag dstAccess) noexcept {

        auto imageMemoryBarrier = VkImageMemoryBarrier {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags> (srcAccess);
        imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags> (dstAccess);
        imageMemoryBarrier.oldLayout = static_cast<VkImageLayout> (oldLayout);
        imageMemoryBarrier.newLayout = static_cast<VkImageLayout> (newLayout);
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = image->getHandle();

        auto subresourceRange = image->getFullRange();

        imageMemoryBarrier.subresourceRange.aspectMask = static_cast<VkImageAspectFlags> (subresourceRange.aspectMask);
        imageMemoryBarrier.subresourceRange.baseArrayLayer = static_cast<std::uint32_t> (subresourceRange.baseArrayLayer);
        imageMemoryBarrier.subresourceRange.layerCount = static_cast<std::uint32_t> (subresourceRange.layerCount);
        imageMemoryBarrier.subresourceRange.baseMipLevel = static_cast<std::uint32_t> (subresourceRange.baseMipLevel);
        imageMemoryBarrier.subresourceRange.levelCount = static_cast<std::uint32_t> (subresourceRange.levelCount);

        vkCmdPipelineBarrier(_handle, static_cast<VkPipelineStageFlags> (srcStageMask), static_cast<VkPipelineStageFlags> (dstStageMask), 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
    }

    void CommandBuffer::copyBuffer(
            const Buffer * src, const Buffer * dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size) noexcept {

        auto region = VkBufferCopy {};
        region.srcOffset = static_cast<VkDeviceSize> (srcOffset);
        region.dstOffset = static_cast<VkDeviceSize> (dstOffset);
        region.size = static_cast<VkDeviceSize> (size);

        vkCmdCopyBuffer(_handle, src->getHandle(), dst->getHandle(), 1, &region);
    }

    void CommandBuffer::pipelineBarrier(
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            DependencyFlag dependencyFlags,
            std::size_t memoryBarrierCount,
            const MemoryBarrier * pMemoryBarriers,
            std::size_t bufferMemoryBarrierCount,
            const BufferMemoryBarrier * pBufferMemoryBarriers,
            std::size_t imageMemoryBarrierCount,
            const ImageMemoryBarrier * pImageMemoryBarriers) noexcept {

        auto memoryBarriers = std::vector<VkMemoryBarrier> ();
        memoryBarriers.reserve(memoryBarrierCount);

        std::for_each(pMemoryBarriers, pMemoryBarriers + memoryBarrierCount, [&](const auto& barrier) {
            auto memoryBarrier = VkMemoryBarrier {};
            memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            memoryBarrier.srcAccessMask = static_cast<VkAccessFlags> (barrier.srcAccessMask);
            memoryBarrier.dstAccessMask = static_cast<VkAccessFlags> (barrier.dstAccessMask);

            memoryBarriers.push_back(memoryBarrier);
        });

        auto bufferMemoryBarriers = std::vector<VkBufferMemoryBarrier> ();
        bufferMemoryBarriers.reserve(bufferMemoryBarrierCount);

        std::for_each(pBufferMemoryBarriers, pBufferMemoryBarriers + bufferMemoryBarrierCount, [&](const auto& barrier) {
            auto bufferMemoryBarrier = VkBufferMemoryBarrier {};
            bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            bufferMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags> (barrier.srcAccessMask);
            bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags> (barrier.dstAccessMask);
            bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            if (barrier.srcQueueFamily) {
                bufferMemoryBarrier.srcQueueFamilyIndex = static_cast<uint32_t> (barrier.srcQueueFamily->getIndex());
            }

            if (barrier.dstQueueFamily) {
                bufferMemoryBarrier.dstQueueFamilyIndex = static_cast<uint32_t> (barrier.dstQueueFamily->getIndex());
            }

            bufferMemoryBarrier.buffer = barrier.buffer->getHandle();
            bufferMemoryBarrier.offset = static_cast<VkDeviceSize> (barrier.offset);
            bufferMemoryBarrier.size = static_cast<VkDeviceSize> (barrier.size);

            bufferMemoryBarriers.push_back(bufferMemoryBarrier);
        });

        auto imageMemoryBarriers = std::vector<VkImageMemoryBarrier> ();
        imageMemoryBarriers.reserve(imageMemoryBarrierCount);

        std::for_each(pImageMemoryBarriers, pImageMemoryBarriers + imageMemoryBarrierCount, [&](const auto& barrier) {
            auto imageMemoryBarrier = VkImageMemoryBarrier {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags> (barrier.srcAccessMask);
            imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags> (barrier.dstAccessMask);
            imageMemoryBarrier.oldLayout = static_cast<VkImageLayout> (barrier.oldLayout);
            imageMemoryBarrier.newLayout = static_cast<VkImageLayout> (barrier.newLayout);
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            if (barrier.srcQueueFamily) {
                imageMemoryBarrier.srcQueueFamilyIndex = static_cast<uint32_t> (barrier.srcQueueFamily->getIndex());
            }

            if (barrier.dstQueueFamily) {
                imageMemoryBarrier.dstQueueFamilyIndex = static_cast<uint32_t> (barrier.dstQueueFamily->getIndex());
            }

            imageMemoryBarrier.image = barrier.image->getHandle();
            imageMemoryBarrier.subresourceRange.aspectMask = static_cast<VkImageAspectFlags> (barrier.subresourceRange.aspectMask);
            imageMemoryBarrier.subresourceRange.baseMipLevel = static_cast<uint32_t> (barrier.subresourceRange.baseMipLevel);
            imageMemoryBarrier.subresourceRange.levelCount = static_cast<uint32_t> (barrier.subresourceRange.levelCount);
            imageMemoryBarrier.subresourceRange.baseArrayLayer = static_cast<uint32_t> (barrier.subresourceRange.baseArrayLayer);
            imageMemoryBarrier.subresourceRange.layerCount = static_cast<uint32_t> (barrier.subresourceRange.layerCount);

            imageMemoryBarriers.push_back(imageMemoryBarrier);
        });

        vkCmdPipelineBarrier(
            _handle, 
            static_cast<VkPipelineStageFlags> (srcStageMask), static_cast<VkPipelineStageFlags> (dstStageMask), static_cast<VkDependencyFlags> (dependencyFlags), 
            memoryBarriers.size(), memoryBarriers.empty() ? nullptr : memoryBarriers.data(),
            bufferMemoryBarriers.size(), bufferMemoryBarriers.empty() ? nullptr : bufferMemoryBarriers.data(),
            imageMemoryBarriers.size(), imageMemoryBarriers.empty() ? nullptr : imageMemoryBarriers.data());
    }
}
