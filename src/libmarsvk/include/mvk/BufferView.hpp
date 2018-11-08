#pragma once

#include <cstddef>

#include "volk.h"

#include <memory>
#include <utility>

#include "mvk/Format.hpp"

namespace mvk {
    class Buffer;
    class Device;

    //! Wrapper to a Vulkan BufferView object.
    /*!
        A buffer view represents a contiguous range of a buffer and a specific format to be used to interpret the data. 
        Buffer views are used to enable shaders to access buffer contents interpreted as formatted data. 
        In order to create a valid buffer view, the buffer must have been created with at least one of the following usage flags:
        * BufferUsageFlag::UNIFORM_TEXEL_BUFFER
        * BufferUsageFlag::STORAGE_TEXEL_BUFFER
    
        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBufferView.html">VkBufferView</a>
    */
    class BufferView {
    public:
        struct CreateInfo {
            int flags;
            Format format;
            std::ptrdiff_t offset;
            std::size_t range;
        };

    private:
        VkBufferView _handle;
        CreateInfo _info;
        const Buffer * _buffer;

        BufferView(const BufferView&) = delete;
        BufferView& operator= (const BufferView&) = delete;

    public:
        std::shared_ptr<void> userData;

        //! Constructs a new BufferView object wrapping nothing.
        BufferView() noexcept:
            _handle(nullptr),
            _buffer(nullptr) {}

        //! Constructs a new BufferView object from a pointer to a Buffer with supplied construction parameters.
        /*!
            \param buffer is the pointer to the Buffer object.
            \param info is the additional parameter struct.
        */
        BufferView(const Buffer * buffer, const CreateInfo& info);

        //! Constructs a new BufferView object from a unique_ptr to a Buffer with supplied construction parameters.
        /*!
            This constructor will unwrap the unique_ptr and chain the raw pointer constructor.

            \param buffer is the unique_ptr to the Buffer object.
            \param info is the additional parameter struct.
        */
        BufferView(const std::unique_ptr<Buffer>& buffer, const CreateInfo& info) :
            BufferView(buffer.get(), info) {}

        //! Move constructs the BufferView object.
        /*!
            \param from the other BufferView.
        */
        BufferView(BufferView&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)), 
            _info(std::move(from._info)),
            _buffer(std::move(from._buffer)) {}

        //! Deletes the BufferView and releases all backing resources.
        ~BufferView() noexcept;

        //! Move assigns the BufferView object.
        /*!
            \param from the other BufferView.
        */
        BufferView& operator= (BufferView&& from) noexcept;

        //! Implicitly cast the BufferView to the underlying Vulkan handle.
        inline operator VkBufferView() const noexcept {
            return _handle;
        }

        //! Retrieve the underlying BufferView.
        /*!
            \return the Vulkan handle.
        */
        inline VkBufferView getHandle() const noexcept {
            return _handle;
        }

        //! Retrieves the information used to construct this BufferView.
        /*!
            \return const reference to an immutable copy of the construction parameter struct.
        */
        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        //! Retrieves the pointer to the Buffer used.
        /*!
            \return the Buffer.
        */
        inline const Buffer * getBuffer() const noexcept {
            return _buffer;
        }

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;
    };
}
