#pragma once

#include "volk.h"

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
        Buffer * _buffer;

    public:
        void * userData;

        BufferView():
            _handle(nullptr) {}

        BufferView(Buffer * buffer, const CreateInfo& info);

        BufferView(const BufferView&) = delete;

        BufferView(BufferView&&) = default;

        ~BufferView();

        BufferView& operator= (const BufferView&) = delete;

        BufferView& operator= (BufferView&&) = default;

        inline VkBufferView getHandle() const {
            return _handle;
        }

        inline const CreateInfo& getInfo() const {
            return _info;
        }

        inline Buffer * getBuffer() const {
            return _buffer;
        }

        Device * getDevice() const;
    };
}