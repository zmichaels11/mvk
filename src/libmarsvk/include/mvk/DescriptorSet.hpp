#pragma once

#include <cstdint>

#include "volk.h"

#include <memory>
#include <utility>

#include "mvk/DescriptorType.hpp"
#include "mvk/ImageLayout.hpp"

namespace mvk {
    class Buffer;
    class Device;
    class DescriptorPool;
    class ImageView;
    class Sampler;

    //! A DescriptorSet object is an interface that exposes bind operations for shader resources.
    class DescriptorSet {
        VkDescriptorSet _handle;
        DescriptorPool * _pool;
        int _poolIndex;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator= (const DescriptorSet&) = delete;

    public:
        //! Creates an empty DescriptorSet object.
        DescriptorSet() noexcept:
            _handle(VK_NULL_HANDLE),
            _pool(nullptr),
            _poolIndex(-1) {}

        //! Constructs a DescriptorSet object by wrapping an externally allocated Vulkan handle.
        /*!
            \param pool the DescriptorPool the DescriptorSet handle was allocated from.
            \param poolIndex the index to the DescriptorPool the DescriptorSet handle was allocated frmo.
            \param handle the Vulkan handle.
        */
        DescriptorSet(DescriptorPool * pool, int poolIndex, VkDescriptorSet handle) noexcept:
            _handle(handle),
            _pool(pool),
            _poolIndex(poolIndex) {}

        //! Move-constructs the DescriptorSet.
        /*!
            \param from the other DescriptorSet.
        */
        DescriptorSet(DescriptorSet&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _pool(std::move(from._pool)),
            _poolIndex(std::move(from._poolIndex)) {}

        //! Move-assigns the DescriptorSet.
        /*!
            \param from the other DescriptorSet.
        */
        DescriptorSet& operator= (DescriptorSet&& from) = default;

        //! Retrieves the DescriptorPool this DescriptorSet was allocated from.
        /*!
            \return the DescriptorPool.
        */
        inline DescriptorPool * getDescriptorPool() const noexcept {
            return _pool;
        }

        //! Retrieves the index of the DescriptorPool this DescriptorSet was allocated from.
        /*!
            \return the DescriptorPool index.
        */
        inline int getPoolIndex() const noexcept {
            return _poolIndex;
        }

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Vulkan handle.
        */
        inline VkDescriptorSet getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkDescriptorSet() const noexcept {
            return _handle;
        }

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Explicitly releases the DescriptorSet.
        /*!
            DescriptorSets are implicitly released by the deconstructor, so it is not recommended to call this method.
        */
        void release();

        //! Writes a Buffer bind to the DescriptorSet.
        /*!
            \param type the type of the descriptor bind. Accepted values are either UNIFORM_BUFFER or STORAGE_BUFFER.
            \param binding is the shader location to bind to.
            \param buffer is the pointer to the Buffer.
            \param offset is the offset in bytes into the Buffer to bind.
            \param range is the length of the bind in bytes.
        */
        void writeBuffer(
            DescriptorType type, int binding, 
            const Buffer * buffer, VkDeviceSize offset = 0L, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        //! Writes a Buffer bind to the DescriptorSet.
        /*!
            This method unwraps the unique_ptr and chains the raw pointer version.
        */
        inline void writeBuffer(
            DescriptorType type, int binding,
            const std::unique_ptr<Buffer>& buffer, VkDeviceSize offset = 0L, VkDeviceSize range = VK_WHOLE_SIZE) noexcept {

            writeBuffer(type, binding, buffer.get(), offset, range);
        }

        //! Writes an Image bind to the DescriptorSet.
        /*!
            \param type is the type of the descriptor bind. Accepted values are COMBINED_IMAGE_SAMPLER, SAMPLED_IMAGE, STORAGE_IMAGE, or INPUT_ATTACHMENT.
            \param binding is the shader location to bind to.
            \param sampler is the pointer to the Sampler object to use. May be nullptr if a Sampler is not expected.
            \param imageLayout is the expected ImageLayout at time of shader execution.
            \param imageView is a pointer to the ImageView.
        */
        void writeImage(
            DescriptorType type, int binding,
            const Sampler * sampler, ImageLayout imageLayout, const ImageView * imageView) noexcept;
    };
}
