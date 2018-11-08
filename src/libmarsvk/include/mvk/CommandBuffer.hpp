#pragma once

#include "volk.h"

#include <memory>
#include <utility>
#include <vector>

#include "mvk/AccessFlag.hpp"
#include "mvk/BufferMemoryBarrier.hpp"
#include "mvk/ClearValue.hpp"
#include "mvk/CommandBufferUsageFlag.hpp"
#include "mvk/CommandBufferLevel.hpp"
#include "mvk/DependencyFlag.hpp"
#include "mvk/Extent2D.hpp"
#include "mvk/Extent3D.hpp"
#include "mvk/Filter.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/ImageMemoryBarrier.hpp"
#include "mvk/ImageSubresourceLayers.hpp"
#include "mvk/IndexType.hpp"
#include "mvk/MemoryBarrier.hpp"
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

    class CommandBuffer;
    using UPtrCommandBuffer = std::unique_ptr<CommandBuffer>;

    //! A buffer of recorded commands that may be executed at a later point.
    /*!
        Command buffers are objects used to record commands which can be subsequently submitted to a 
        device queue for execution. There are two levels of command buffers - primary command buffers, 
        which can execute secondary command buffers, and which are submitted to queues, and secondary 
        command buffers, which can be executed by primary command buffers, and which are not directly 
        submitted to queues.

        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCommandBuffer.html">VkCommandBuffer</a>
    */
    class CommandBuffer {
        CommandPool * _pool;
        CommandBufferLevel _level;
        VkCommandBuffer _handle;

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;

    public:
        //! Constructs a CommandBuffer typed unique_ptr pointing to nothing.
        /*!
            \return the CommandBuffer-typed unique_ptr pointing to nullptr.
        */
        static inline UPtrCommandBuffer unique_null() {
            return std::unique_ptr<CommandBuffer>();
        }

        //! Constructs a CommandBuffer holding nothing.
        CommandBuffer() noexcept:
            _pool(nullptr),
            _handle(VK_NULL_HANDLE) {}

        //! Constructs a new CommandBuffer.
        /*!
            \param pool pointer to the CommandPool to allocate the CommandBuffer from.
            \param level the intended CommandBuffer level.
            \param handle the Vulkan handle to wrap.
        */
        CommandBuffer(CommandPool * pool, CommandBufferLevel level, VkCommandBuffer handle) noexcept:
            _pool(pool),
            _level(level),
            _handle(handle) {}

        //! Move-constructs the CommandBuffer.
        /*!
            \param from the other CommandBuffer.
        */
        CommandBuffer(CommandBuffer&& from) noexcept:
            _pool(std::move(from._pool)),
            _level(std::move(from._level)),
            _handle(std::exchange(from._handle, nullptr)) {}

        //! Move-assigns the CommandBuffer.
        /*!
            \param from the other CommandBuffer.
        */
        CommandBuffer& operator= (CommandBuffer&& from) noexcept;

        //! Deletes the CommandBuffer and releases all resources.
        ~CommandBuffer() noexcept;

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkCommandBuffer() const noexcept {
            return _handle;
        }

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Resets the CommandBuffer.
        void reset(unsigned int flags = 0);

        //! Retrieves the parent CommandPool.
        /*!
            \return the parent CommandPool.
        */
        inline CommandPool * getCommandPool() const noexcept {
            return _pool;
        }

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the handle.
        */
        inline VkCommandBuffer getHandle() const noexcept {
            return _handle;
        }

        //! Retrieves the level of the CommandBuffer.
        /*!
            \return the level.
        */
        inline CommandBufferLevel getLevel() const noexcept {
            return _level;
        }

        void beginRenderPass(const Framebuffer * framebuffer, SubpassContents contents = SubpassContents::INLINE) noexcept;

        inline void beginRenderPass(const std::unique_ptr<Framebuffer>& framebuffer, SubpassContents contents = SubpassContents::INLINE) noexcept {
            beginRenderPass(framebuffer.get(), contents);
        }

        inline void beginRenderPass(const Framebuffer& framebuffer, SubpassContents contents = SubpassContents::INLINE) noexcept {
            beginRenderPass(&framebuffer, contents);
        }

        void endRenderPass() noexcept;

        void nextSubpass(SubpassContents contents = SubpassContents::INLINE) noexcept;

        void setViewport(float x, float y, float width, float height, float minDepth, float maxDepth) noexcept;

        inline void setViewport(float x, float y, float width, float height) noexcept {
            setViewport(x, y, width, height, 1.0F, 1.0F);
        }

        inline void setViewport(const Viewport& viewport) noexcept {
            setViewport(viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth);
        }

        void setScissor(int x, int y, int width, int height) noexcept;

        inline void setScissor(const Offset2D& offset, const Extent2D& extent) noexcept{
            setScissor(offset.x, offset.y, extent.width, extent.height);
        }

        inline void setScissor(const Rect2D& scissor) noexcept{
            setScissor(scissor.offset, scissor.extent);
        }

        void bindVertexBuffer(int binding, const Buffer * buffer, std::ptrdiff_t offset = 0) noexcept;

        inline void bindVertexBuffer(int binding, const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset = 0) noexcept {
            bindVertexBuffer(binding, buffer.get(), offset);
        }

        void bindIndexBuffer(const Buffer * buffer, std::ptrdiff_t offset, IndexType indexType) noexcept;

        inline void bindIndexBuffer(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, IndexType indexType) noexcept {
            bindIndexBuffer(buffer.get(), offset, indexType);
        }

        void draw(int vertexCount, int instanceCount = 1, int firstVertex = 0, int firstInstance = 0) noexcept;

        void drawIndexed(int indexCount, int instanceCount = 1, int firstIndex = 0, int vertexOffset = 0, int firstInstance = 0) noexcept;

        void updateBuffer(const Buffer * buffer, std::ptrdiff_t offset, const void * pData) noexcept;

        inline void updateBuffer(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, const void * pData) noexcept{
            updateBuffer(buffer.get(), offset, pData);
        }

        void begin(CommandBufferUsageFlag flags);

        void bindDescriptorSet(const Pipeline * pipeline, int set, const DescriptorSet * descriptorSet) noexcept;

        template<class PipelineT>
        inline void bindDescriptorSet(const std::unique_ptr<PipelineT>& pipeline, int set, const DescriptorSet * descriptorSet) noexcept{
            bindDescriptorSet(pipeline.get(), set, descriptorSet);
        }

        void bindDescriptorSet(const Pipeline * pipeline, int set, const DescriptorSet * descriptorSet, std::size_t nDynamicOffsets, const int * pDynamicOffsets) noexcept;

        template<class PipelineT>
        inline void bindDescriptorSet(const std::unique_ptr<PipelineT>& pipeline, int set, const DescriptorSet * descriptorSet, std::size_t nDynamicOffsets, const int * pDynamicOffsets) noexcept {
            bindDescriptorSet(pipeline.get(), set, descriptorSet, nDynamicOffsets, pDynamicOffsets);
        }

        void bindPipeline(const Pipeline * pipeline) noexcept;

        template<class PipelineT>
        inline void bindPipeline(const std::unique_ptr<PipelineT>& pipeline) noexcept {
            bindPipeline(pipeline.get());
        }

        void dispatch(unsigned int groupsX, unsigned int groupsY = 1, unsigned int groupsZ = 1) noexcept;

        void dispatchIndirect(const Buffer * buffer, std::ptrdiff_t offset) noexcept;

        inline void dispatchIndirect(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset) noexcept {
            dispatchIndirect(buffer.get(), offset);
        }

        void drawIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept;

        inline void drawIndirect(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept {
            drawIndirect(buffer.get(), offset, drawCount, stride);
        }

        void drawIndexedIndirect(const Buffer * buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept;

        inline void drawIndexedIndirect(const std::unique_ptr<Buffer>& buffer, std::ptrdiff_t offset, int drawCount, int stride) noexcept {
            drawIndexedIndirect(buffer.get(), offset, drawCount, stride);
        }

        void end();
        
        void pushConstants(const Pipeline * pipeline, ShaderStage stages, std::ptrdiff_t , std::size_t size, const void * data) noexcept;

        template<class PipelineT>
        inline void pushConstants(const std::unique_ptr<PipelineT>& pipeline, ShaderStage stages, std::ptrdiff_t offset, std::size_t size, const void * data) noexcept {
            pushConstants(pipeline.get(), stages, offset, size, data);
        }

        void copyBufferToImage(
            const Buffer * src, std::ptrdiff_t bufferOffset, 
            const Image * dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent) noexcept;

        inline void copyBufferToImage(
            const std::unique_ptr<Buffer>& src, std::ptrdiff_t bufferOffset,
            const std::unique_ptr<Image>& dst, ImageLayout layout, 
            const ImageSubresourceLayers& subresourceRange, 
            const Offset3D& offset, const Extent3D& extent) noexcept {

            copyBufferToImage(src.get(), bufferOffset, dst.get(), layout, subresourceRange, offset, extent);
        }

        void copyImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource) noexcept;

        inline void copyImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const Offset3D& srcOffset, const Offset3D& dstOffset, const Extent3D& extent,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource) noexcept {

            copyImage(src.get(), srcLayout, dst.get(), dstLayout, srcOffset, dstOffset, extent, srcSubresource, dstSubresource);
        }

        void blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) noexcept;

        inline void blitImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const Offset3D& srcOffset0, const Offset3D& srcOffset1,
            const Offset3D& dstOffset0, const Offset3D& dstOffset1,
            const ImageSubresourceLayers& srcSubresource, 
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) noexcept {

            blitImage(src.get(), srcLayout, dst.get(), dstLayout, srcOffset0, srcOffset1, dstOffset0, dstOffset1, srcSubresource, dstSubresource, filter);
        }

        void blitImage(
            const Image * src, ImageLayout srcLayout,
            const Image * dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) noexcept;
            
        inline void blitImage(
            const std::unique_ptr<Image>& src, ImageLayout srcLayout,
            const std::unique_ptr<Image>& dst, ImageLayout dstLayout,
            const ImageSubresourceLayers& srcSubresource,
            const ImageSubresourceLayers& dstSubresource,
            Filter filter = Filter::LINEAR) noexcept {

            blitImage(src.get(), srcLayout, dst.get(), dstLayout, srcSubresource, dstSubresource, filter);
        }

        void stageImage(
            const Image * image, 
            ImageLayout oldLayout, ImageLayout newLayout,
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            AccessFlag srcAccess, AccessFlag dstAccess) noexcept;

        inline void stageImage(
            const std::unique_ptr<Image>& image, 
            ImageLayout oldLayout, ImageLayout newLayout,
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            AccessFlag srcAccess, AccessFlag dstAccess) noexcept {

            stageImage(image.get(), oldLayout, newLayout, srcStageMask, dstStageMask, srcAccess, dstAccess);
        }

        void copyBuffer(
            const Buffer * src, const Buffer * dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size) noexcept;

        inline void copyBuffer(
            const std::unique_ptr<Buffer>& src, const std::unique_ptr<Buffer>& dst,
            std::ptrdiff_t srcOffset, std::ptrdiff_t dstOffset,
            std::size_t size) noexcept {

            copyBuffer(src.get(), dst.get(), srcOffset, dstOffset, size);
        }

        void pipelineBarrier(
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            DependencyFlag dependencyFlags,
            std::size_t memoryBarrierCount,
            const MemoryBarrier * pMemoryBarriers,
            std::size_t bufferMemoryBarrierCount,
            const BufferMemoryBarrier * pBufferMemoryBarriers,
            std::size_t imageMemoryBarrierCount,
            const ImageMemoryBarrier * pImageMemoryBarriers) noexcept;

        inline void pipelineBarrier(
            PipelineStageFlag srcStageMask, PipelineStageFlag dstStageMask,
            DependencyFlag dependencyFlags,
            const std::vector<MemoryBarrier>& memoryBarriers,
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers,
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) noexcept {

            pipelineBarrier(
                srcStageMask, dstStageMask, 
                dependencyFlags, 
                memoryBarriers.size(), memoryBarriers.data(), 
                bufferMemoryBarriers.size(), bufferMemoryBarriers.data(), 
                imageMemoryBarriers.size(), imageMemoryBarriers.data());
        }
    };
}
