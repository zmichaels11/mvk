#pragma once

#include "volk.h"

#include <memory>

#include "mvk/Format.hpp"

namespace mvk {
    class Buffer;
    class Device;

    class BufferView {
    public:
        struct CreateInfo {
            int flags;
            Format format;
            VkDeviceSize offset;
            VkDeviceSize range;
        };

    private:
        VkBufferView _handle;
        CreateInfo _info;
        const Buffer * _buffer;

    public:
        void * userData;

        BufferView():
            _handle(nullptr) {}

        BufferView(const Buffer * buffer, const CreateInfo& info);

        BufferView(const std::unique_ptr<Buffer>& buffer, const CreateInfo& info) :
            BufferView(buffer.get(), info) {}

        BufferView(const BufferView&) = delete;

        BufferView(BufferView&&) = default;

        ~BufferView();

        BufferView& operator= (const BufferView&) = delete;

        BufferView& operator= (BufferView&&) = default;

        inline VkBufferView getHandle() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline const Buffer * getBuffer() const noexcept {
            return _buffer;
        }

        Device * getDevice() const;
    };
}
