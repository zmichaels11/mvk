#pragma once

#include "volk.h"

#include <memory>
#include <vector>

#include "mvk/AccessFlag.hpp"
#include "mvk/ClearValue.hpp"
#include "mvk/CommandBufferUsageFlag.hpp"
#include "mvk/CommandBufferLevel.hpp"
#include "mvk/Extent2D.hpp"
#include "mvk/Extent3D.hpp"
#include "mvk/Filter.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/IndexType.hpp"
#include "mvk/PipelineStageFlag.hpp"
#include "mvk/Offset2D.hpp"
#include "mvk/Offset3D.hpp"
#include "mvk/Rect2D.hpp"
#include "mvk/ShaderStage.hpp"
#include "mvk/SubpassContents.hpp"
#include "mvk/Viewport.hpp"

namespace mvk {
    class Buffer;
    class CommandPool;
    class DescriptorSet;
    class Device;
    class Framebuffer;
    class Image;
    class Pipeline;
    class RenderPass;

    class CommandBuffer {
    public:
        struct RenderPassBeginInfo {
            Framebuffer * framebuffer;
            RenderPass * renderPass;
            SubpassContents subpassContents;
            Rect2D renderArea;
            std::vector<ClearValue> clearValues;
        };

    private:
        CommandPool * _pool;
        CommandBufferLevel _level;
        VkCommandBuffer _handle;

    public:
        CommandBuffer(CommandPool * pool, CommandBufferLevel level, VkCommandBuffer handle) :
            _pool(pool),
            _level(level),
            _handle(handle) {}

        ~CommandBuffer();

        Device * getDevice() const;

        void reset(unsigned int flags = 0);

        inline CommandPool * getCommandPool() const noexcept {
            return _pool;
        }

        inline VkCommandBuffer getHandle() const noexcept {
            return _handle;
        }

        inline CommandBufferLevel getLevel() const noexcept {
            return _level;
        }

        void beginRenderPass(const RenderPassBeginInfo& beginInfo);

        void endRenderPass();

        void nextSubpass(SubpassContents contents = SubpassContents::INLINE);

        void setViewport(float x, float y, float width, float height, float minDepth, float maxDepth);

        inline void setViewport(float x, float y, float width, float height) {
            setViewport(x, y, width, height, 1.0F, 1.0F);
        }

        inline void setViewport(const Viewport& viewport) {
            setViewport(viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth);
        }

        void setScissor(int x, int y, int width, int height);

        inline void setScissor(const Offset2D& offset, const Extent2D& extent) {
            setScissor(offset.x, offset.y, extent.width, extent.height);
        }

        inline void setScissor(const Rect2D& scissor) {
            setScissor(scissor.offset, scissor.extent);
        }

        void bindVertexBuffer(int binding, const Buffer * buffer, std::ptrdiff_t offset = 0);

        inline void bindVertexBuffer(int binding, const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset = 0) {
            bindVertexBuffer(binding, buffer.get(), offset);
        }

        void bindVertexBuffers(int firstBinding, const std::vector<const Buffer *>& buffers, const std::vector<std::ptrdiff_t>& offsets);

        void bindIndexBuffer(const Buffer * buffer, std::ptrdiff_t offset, IndexType indexType);

        inline void bindIndexBuffer(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, IndexType indexType) {
            bindIndexBuffer(buffer.get(), offset, indexType);
        }

        void draw(int vertexCount, int instanceCount = 1, int firstVertex = 0, int firstInstance = 0);

        void drawIndexed(int indexCount, int instanceCount = 1, int firstIndex = 0, int vertexOffset = 0, int firstInstance = 0);

        void updateBuffer(const Buffer * buffer, std::ptrdiff_t offset, const void * pData);

        inline void updateBuffer(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, const void * pData) {
            updateBuffer(buffer.get(), offset, pData);
        }

        void begin(CommandBufferUsageFlag flags);

        void bindDescriptorSet(const Pipeline * pipeline, int firstSet, const DescriptorSet * descriptorSet);

        template<class PipelineT>
        inline void bindDescriptorSet(const std::unique_ptr<PipelineT>& pipeline, int firstSet, const DescriptorSet * descriptorSet) {
            bindDescriptorSet(pipeline.get(), firstSet, descriptorSet);
        }

        void bindPipeline(const Pipeline * pipeline);

        template<class PipelineT>
        inline void bindPipeline(std::unique_ptr<PipelineT>& pipeline) {
            bindPipeline(pipeline.get());
        }

        void dispatch(unsigned int groupsX, unsigned int groupsY = 1, unsigned int groupsZ = 1);

        void dispatchIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride);

        inline void dispatchIndirect(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, int drawCount, int stride) {
            dispatchIndirect(buffer.get(), offset, drawCount, stride);
        }

        void dispatchIndexedIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride);

        inline void dispatchIndexedIndirect(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, int drawCount, int stride) {
            dispatchIndexedIndirect(buffer.get(), offset, drawCount, stride);
        }

        void end();
        
        void pushConstants(const Pipeline * pipeline, ShaderStage stages, int offset, const void * data);

        void copyBufferToImage(
            const Buffer * src, std::ptrdiff_t bufferOffset, 
            const Image * dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent);

        inline void copyBufferToImage(
            const std::unique_ptr<Buffer>& src, std::ptrdiff_t bufferOffset,
            const std::unique_ptr<Image>& dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent) {

            copyBufferToImage(src.get(), bufferOffset, dst.get(), layout, subresourceRange, offset, extent);
        }

        void copyImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource);

        inline void copyImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource) {

            copyImage(src.get(), srcLayout, dst.get(), dstLayout, srcOffset, dstOffset, extent, srcSubresource, dstSubresource);
        }

        void blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR);

        inline void blitImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) {

            blitImage(src.get(), srcLayout, dst.get(), dstLayout, srcOffset0, srcOffset1, dstOffset0, dstOffset1, srcSubresource, dstSubresource, filter);
        }

        void blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR);
            
        inline void blitImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) {

            blitImage(src.get(), srcLayout, dst.get(), dstLayout, srcSubresource, dstSubresource, filter);
        }

        void stageImage(
            const Image * image, 
            ImageLayout oldLayout, ImageLayout newLayout,
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            AccessFlag srcAccess, AccessFlag dstAccess);

        inline void stageImage(
            const std::unique_ptr<Image>& image, 
            ImageLayout oldLayout, ImageLayout newLayout,
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            AccessFlag srcAccess, AccessFlag dstAccess) {

            stageImage(image.get(), oldLayout, newLayout, srcStageMask, dstStageMask, srcAccess, dstAccess);
        }

        void copyBuffer(
            const Buffer * src, const Buffer * dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size);

        inline void copyBuffer(
            const std::unique_ptr<Buffer>& src, const std::unique_ptr<Buffer>& dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size) {

            copyBuffer(src.get(), dst.get(), srcOffset, dstOffset, size);
        }
    };
}
